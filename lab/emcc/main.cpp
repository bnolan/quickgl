#include <emscripten.h>
#include <emscripten/html5.h>
#include <GL/glut.h>

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    glFlush();
}

int main() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL Demo");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glutDisplayFunc(render);
    emscripten_set_main_loop(render, 0, 1);
    return 0;
}