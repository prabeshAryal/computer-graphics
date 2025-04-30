#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

void display() {
	glClearColor(1.0f, 0.9f, 0.9f,1.0f);
	glPushMatrix();
	glTranslatef(0.0f,-0.2f,0.0f);


	// chimney
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

	glColor3f(0.0f, 0.0f, 0.0f); // Set rectangle color to red
	glBegin(GL_QUADS); // Start drawing a rectangle (quadrilateral)
	glVertex2f(0.1f, 0.7); // Bottom-left
	glVertex2f(0.3f, 0.7); // Bottom-right
	glVertex2f(0.3f, 1.1f); // Top-right
	glVertex2f(0.1f, 1.1f); // Top-left
	glEnd();

	glColor3f(0.3f, 0.3f, 0.3f); // Set rectangle color to gray
	// Roof
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, 0.5f); // bottom-left
	glVertex2f(0.5f, 0.5f); // bottom-right
	glVertex2f(0.0f, 1.2f); // Top-right

		glEnd();

		//House Base
	//glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
	glColor3f(1.0f, 1.0f, 0.0f); // Set rectangle color to red
	glBegin(GL_QUADS); // Start drawing a rectangle (quadrilateral)
	glVertex2f(-0.5f, -0.5f); // Bottom-left
	glVertex2f(0.5f, -0.5f); // Bottom-right
	glVertex2f(0.5f, 0.5f); // Top-right
	glVertex2f(-0.5f, 0.5f); // Top-left
	glEnd();

	//Door
	glColor3f(1.0f, 1.0f, 1.0f); // Set rectangle color to red
	glBegin(GL_QUADS); // Start drawing a rectangle (quadrilateral)
	glVertex2f(-0.15f, -0.5f); // Bottom-left
	glVertex2f(0.15f, -0.5f); // Bottom-right
	glVertex2f(0.15f, 0.0f); // Top-right
	glVertex2f(-0.15f, 0.0f); // Top-left
	glEnd();




	glFlush(); // Render the rectangle
}


int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);

	// Set the window size
	glutInitWindowSize(1000, 1000);
	
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello, GL");
	// Bind the two functions (above) to respond when necessary
	glutDisplayFunc(display);

	// To avoid window to be closed
	glutMainLoop();
	return 0;
}