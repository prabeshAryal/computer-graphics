#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

void display() {

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.7f, -0.6f);
	glVertex2f(0.4f, -0.1f);

	glEnd();

	glFlush();
}


int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);

	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello, GL");
	// Bind the two functions (above) to respond when necessary
	glutDisplayFunc(display);

	// To avoid window to be closed
	glutMainLoop();
	return 0;
}