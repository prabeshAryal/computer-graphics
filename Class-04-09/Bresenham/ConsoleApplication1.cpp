#include <Windows.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Global coordinates
int x_start, y_start;
int x_end, y_end;

// Bresenham Line Drawing (pixel-perfect)
void drawBresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        glVertex2i(x1, y1);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glPointSize(1.5);

    glBegin(GL_POINTS);
    drawBresenhamLine(x_start, y_start, x_end, y_end);
    glEnd();

    glFlush();
}

// OpenGL init
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1366, 0, 768); // Set 2D coordinate system
}

// Main function
int main(int argc, char* argv[]) {
    cout << "Bresenham Line Drawing - White on Black\n";
    cout << "----------------------------------------\n";
    cout << "Enter start point (x1 y1) [0-1366 0-768]: ";
    cin >> x_start >> y_start;
    cout << "Enter end point (x2 y2) [0-1366 0-768]: ";
    cin >> x_end >> y_end;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
