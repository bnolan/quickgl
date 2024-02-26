#include <emscripten.h>
#include <emscripten/html5.h>
#include <GL/glut.h>
#include <opencv2/opencv.hpp>

using namespace cv;

// Generate procedural texture using OpenCV
Mat generateProceduralTexture(int width, int height) {
    Mat texture(height, width, CV_8UC3);

    // Generate procedural texture (e.g., noise, gradients, patterns)
    // Here, we generate a simple gradient from blue to green
    for (int y = 0; y < texture.rows; ++y) {
        for (int x = 0; x < texture.cols; ++x) {
            texture.at<Vec3b>(y, x) = Vec3b(255 * y / height, 0, 255 * (1 - y / height));
        }
    }

    return texture;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the procedural texture on a quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    glFlush();
}

int main() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL Demo");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background

    // Generate procedural texture
    Mat texture = generateProceduralTexture(512, 512);

    // Initialize OpenGL texture
    glEnable(GL_TEXTURE_2D);
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.cols, texture.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture coordinates
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    // Set viewport and projection matrix
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // Set display function
    glutDisplayFunc(render);

    emscripten_set_main_loop(render, 0, 1);
    return 0;
}