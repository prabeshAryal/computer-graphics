#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Initialize OpenGL settings
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set background color to white

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Set the coordinate system

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

    // Your drawing code goes here
    // For example:
    glColor3f(0.0f, 0.0f, 0.0f);  // Black color
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();  // Swap the front and back buffers
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Boilerplate");

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    // Set up OpenGL state
    init();

    // Register callbacks
    glutDisplayFunc(display);

    // Start the main loop
    glutMainLoop();

    return 0;
}