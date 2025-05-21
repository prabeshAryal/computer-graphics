#include <GL/glut.h> // Required for OpenGL and GLUT functions
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
    GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(2.0, 2.0, 5.0, // Camera is at (2, 2, 5)
              0.0, 0.0, 0.0, // Looking towards the origin (0, 0, 0)
              0.0, 1.0, 0.0); // Y-axis is the up direction
    glColor3f(0.6, 0.8, 1.0);
    glutSolidCube(2.0);
    glFlush();
    glutSwapBuffers();
}
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Flat Shaded Cube - OpenGL (GLUT)");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
