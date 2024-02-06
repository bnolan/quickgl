#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

extern "C" {
#include "quickjs.h"
}

std::string getCounterText(JSContext *ctx, int frameCount) {
  // Define the JavaScript code that generates the counter text
  std::string jsCode = R"XX(

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

  jsCode += "getCounterText(" + std::to_string(frameCount) + ");";

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

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    return 4;
  }

  // QuickJS

  // Initialize the QuickJS runtime and context
  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  // Your JavaScript code as a string
  const char *jsCode = "2 + 2";

  // Evaluate the JavaScript code
  JSValue result =
      JS_Eval(ctx, jsCode, strlen(jsCode), "<evalScript>", JS_EVAL_TYPE_GLOBAL);

  const char *text;

  // Check if the evaluation was successful
  if (JS_IsException(result)) {
    std::cerr << "JavaScript evaluation resulted in an exception." << std::endl;
  } else {
    // Convert the result to a C string and print it
    text = JS_ToCString(ctx, result);
    std::cout << "Result of `" << jsCode << "` is " << text << std::endl;
    // JS_FreeCString(ctx, resultStr); // Free the C string
  }

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
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);

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
