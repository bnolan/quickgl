#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

extern "C" {

#define TGL_FEATURE_16_BITS 1
#include "GL/gl.h"
#include "quickjs.h"
#include "zbuffer.h"
#define CHAD_API_IMPL
#define CHAD_MATH_IMPL
#include "3dMath.h"
}

double time_passed = 0.0;
int winSizeX = 480;
int winSizeY = 480;

void draw() {
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glEnable(GL_TEXTURE_2D);
  // glBindTexture(GL_TEXTURE_2D,tex);
  // time_passed += 0.0166666;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  glRotatef(time_passed * 90, 0, 0, 1);
  glBegin(GL_TRIANGLES);
  glColor3f(0.2, 0.2, 0.2);
  glVertex3f(-0.8, -0.8, 0.2);

  glColor3f(0.5, 0.5, 0.5);
  glVertex3f(0.8, -0.8, 0.2);

  glColor3f(0.9, 0.9, 0.9);
  glVertex3f(0, 1.2, 0.2);
  glEnd();
  glPopMatrix();
}

void initScene() {

  // glLightfv( GL_LIGHT0, GL_AMBIENT, white);
  // glLightfv( GL_LIGHT0, GL_SPECULAR, white);
  // glEnable( GL_CULL_FACE );
  glDisable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  // glEnable( GL_LIGHT0 );
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glTextSize(GL_TEXT_SIZE24x24);
  /*
  {
          int sw = 0, sh = 0, sc = 0; //sc goes unused.
          uchar* source_data = stbi_load("texture.png", &sw, &sh, &sc, 3);
          if(source_data){
                  tex = loadRGBTexture(source_data, sw, sh);
                  free(source_data);
          } else {
                  printf("\nCan't load texture!\n");
          }
  }*/
  glEnable(GL_NORMALIZE);
}

// Wrapper for glColor3f
static JSValue js_glColor3f(JSContext *ctx, JSValueConst this_val, int argc,
                            JSValueConst *argv) {
  double r, g, b;

  // Extract the arguments from JavaScript
  JS_ToFloat64(ctx, &r, argv[0]);
  JS_ToFloat64(ctx, &g, argv[1]);
  JS_ToFloat64(ctx, &b, argv[2]);

  // Call the actual TinyGL function
  glColor3f((float)r, (float)g, (float)b);

  return JS_UNDEFINED;
}

// Wrapper for glVertex3f
static JSValue js_glVertex3f(JSContext *ctx, JSValueConst this_val, int argc,
                             JSValueConst *argv) {
  double x, y, z;

  // Extract the arguments from JavaScript
  JS_ToFloat64(ctx, &x, argv[0]);
  JS_ToFloat64(ctx, &y, argv[1]);
  JS_ToFloat64(ctx, &z, argv[2]);

  // Call the actual TinyGL function
  glVertex3f((float)x, (float)y, (float)z);

  return JS_UNDEFINED;
}

void js_tinygl_init(JSContext *ctx, JSValue ns) {
  // Register each function manually
  JS_SetPropertyStr(ctx, ns, "glColor3f",
                    JS_NewCFunction(ctx, js_glColor3f, "glColor3f", 3));
  JS_SetPropertyStr(ctx, ns, "glVertex3f",
                    JS_NewCFunction(ctx, js_glVertex3f, "glVertex3f", 3));
  // Add more functions as needed
}

std::string getCounterText(JSContext *ctx, int frameCount) {
  std::string jsCode = "getCounterText(" + std::to_string(frameCount) + ");";

  // Evaluate the JavaScript code
  JSValue result = JS_Eval(ctx, jsCode.c_str(), jsCode.length(), "<input>",
                           JS_EVAL_TYPE_GLOBAL);

  // Convert the result to a C string
  const char *resultStr = JS_ToCString(ctx, result);
  if (!resultStr) {
    JS_FreeValue(ctx, result);
    return "JS Error";
  }

  // Convert the result to a std::string and free the JS string
  std::string counterText(resultStr);
  JS_FreeCString(ctx, resultStr);
  JS_FreeValue(ctx, result);

  return counterText;
}

int main() {
  std::cout << "Hello, CMake World!" << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  if (TTF_Init() == -1) {
    SDL_Log("TTF_Init: %s\n", TTF_GetError());
    return 2;
  }

  SDL_Window *window = SDL_CreateWindow("QuickGL", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 480, 480, 0);

  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return 3;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    return 4;
  }

  SDL_Texture *albedoTexture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 480, 480);

  // Initialize TinyGL
  ZBuffer *zbuffer = ZB_open(480, 480, ZB_MODE_5R6G5B, NULL);

  if (!zbuffer) {
    SDL_Log("Failed to create zbuffer");
    return 3;
  }

  glInit(zbuffer);

  // QuickJS

  // Initialize the QuickJS runtime and context
  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  JSValue tinyglNamespace = JS_NewObject(ctx);
  js_tinygl_init(ctx, tinyglNamespace);
  JS_SetPropertyStr(ctx, JS_GetGlobalObject(ctx), "TinyGL", tinyglNamespace);

  // If adding to the global namespace:
  js_tinygl_init(ctx, JS_GetGlobalObject(ctx));

  // Define the JavaScript code that generates the counter text
  std::string jsCode = R"XX(

    function triangle () {
      TinyGL.glColor3f(1.0, 0.0, 0.0); // Set color to red
      TinyGL.glVertex3f(0.0, 0.0, 0.0); // Specify a vertex
    }

    function triangleWave(x, period, min, max) {
      const amplitude = max - min;
      const phase = x % period;
      const ascending = 2 * phase / period;
      if (ascending < 1) {
        return min + amplitude * ascending;
      } else {
        return max - amplitude * (ascending - 1);
      }
    }

    function sineWave(x, period, min, max) {
      return (Math.sin(x / period * 3.14) + 1) * 0.5 * (max-min) + min * 2
    }

    function getCounterText(count) { 
      var width = 20
      var position = sineWave(count, 30, 0, width)
      return "lol: " + ('-'.repeat(position) + '#' + '-'.repeat(width - position))
    }; 

)XX";

  // Evaluate the JavaScript code
  JSValue result = JS_Eval(ctx, jsCode.c_str(), jsCode.length(), "<input>",
                           JS_EVAL_TYPE_GLOBAL);

  // Render...

  TTF_Font *font =
      TTF_OpenFont("../fonts/dosis.ttf", 24); // Adjust font path and size
  if (!font) {
    SDL_Log("Failed to load font: %s", TTF_GetError());
    // Cleanup SDL and QuickJS
    return 5;
  }

  // Cleanup the surface as soon as it's no longer needed
  // SDL_FreeSurface(surface);

  // Cleanup
  // SDL_DestroyTexture(texture);
  // TTF_CloseFont(font);

  bool running = true;
  SDL_Event event;

  int frameCount = 0;

  while (running) {
    // Off black
    // SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    // SDL_RenderClear(renderer);

    // TinyGL rendering commands
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    draw();

    // Copy the TinyGL framebuffer to the SDL texture
    void *pixels;
    int pitch;

    SDL_LockTexture(albedoTexture, NULL, &pixels, &pitch);
    memcpy(pixels, zbuffer->pbuf, 480 * 480 * 2);
    SDL_UnlockTexture(albedoTexture);

    // Render the SDL texture
    SDL_RenderCopy(renderer, albedoTexture, NULL, NULL);

    // Set text to render
    std::string counterText = getCounterText(ctx, frameCount++);

    // Off white
    SDL_Color textColor = {220, 220, 220, 255};
    SDL_Surface *textSurface =
        TTF_RenderText_Blended(font, counterText.c_str(), textColor);
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    // Render text
    SDL_Rect renderQuad = {50, 50, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Update screen
    SDL_RenderPresent(renderer);

    // Free resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_Delay(1000 / 60); // Approximately 60 frames per second

    time_passed += 1.0 / 60;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
  }

  // Cleanup SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  // Free the result value
  JS_FreeValue(ctx, result);

  // Perform cleanup
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);

  return 0;
}
