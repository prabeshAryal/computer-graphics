#include <Windows.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Global coordinates
int x_start, y_start;
int x_end, y_end;

// DDA Line Drawing (pixel-perfect)
void drawDDALine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));  // Round to pixel center
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f); // White

    glPointSize(1.5); // Thin line effect
    drawDDALine(x_start, y_start, x_end, y_end);

    glFlush();
}

// OpenGL init
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1366, 0, 768); // Map coordinates to window size
}

// Main
int main(int argc, char* argv[]) {
    cout << "DDA Line Drawing - White on Black\n";
    cout << "----------------------------------\n";
    cout << "Enter start point (x1 y1) [0-1366 0-768]: ";
    cin >> x_start >> y_start;
    cout << "Enter end point (x2 y2) [0-1366 0-768]: ";
    cin >> x_end >> y_end;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1366,768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
