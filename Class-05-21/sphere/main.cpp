#include <GL/glut.h> // Required for OpenGL and GLUT functions
#include <cmath>     // Required for sin, cos, etc. if needed for more complex rotations, though not strictly for simple ones here.
static GLfloat rotationAngle = 0.0;
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    GLfloat light_pos[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    GLfloat ambient_light[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    GLfloat diffuse_light[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    GLfloat shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(2.0, 2.0, 5.0, // Camera is at (2, 2, 5)
              0.0, 0.0, 0.0, // Looking towards the origin (0, 0, 0)
              0.0, 1.0, 0.0); // Y-axis is the up direction
    glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    glColor3f(0.5, 0.5, 1.0); // A beautiful, vibrant magenta/purple hue
    glutSolidSphere(1.0, 50, 50);
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
void idle() {
    rotationAngle += 0.9; // Rotate by 0.1 degrees per frame (slower than 0.5)
    if (rotationAngle > 360.0) {
        rotationAngle -= 360.0; // Keep the angle within 0-360 degrees
    }
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Rotating Shaded Sphere - OpenGL (GLUT)");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
