#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

void drawPentagon(float centerX, float centerY, float radius, float rotationAngle) {
    const int numVertices = 5;
    const float angleIncrement = 2.0f * 3.14159f / numVertices;
    
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    // Draw the pentagon
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; ++i) {
        float angle = i * angleIncrement;
        // Starting with angle = -PI/2 makes the pentagon point downward
        angle += -3.14159f / 2.0f;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    // Draw direction indicator (arrow)
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the direction indicator
    
    // Line from center to bottom point (main direction)
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f); // Center
    glVertex2f(0.0f, -radius * 1.2f); // Direction indicator pointing down
    glEnd();
    
    // Arrow head
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, -radius * 1.2f); // Tip of arrow
    glVertex2f(-0.05f, -radius * 1.1f); // Left side
    glVertex2f(0.05f, -radius * 1.1f); // Right side
    glEnd();
    
    // Draw coordinate axes for reference
    glBegin(GL_LINES);
    // X-axis (red)
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(radius * 0.8f, 0.0f);
    
    // Y-axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, radius * 0.8f);
    glEnd();
    
    glPopMatrix();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);  
    
    // First pentagon - pointing downward (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    drawPentagon(-0.4f, 0.0f, 0.3f, 0.0f);
    
    // Second pentagon - rotated 45 degrees (purple)
    glColor3f(1.0f, 0.0f, 1.0f);
    drawPentagon(0.4f, 0.0f, 0.3f, 45.0f);
    
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pentagon Display with Direction");
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}