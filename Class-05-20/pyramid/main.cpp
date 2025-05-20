#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>

float rotationAngle = 0.0f;

void initLighting() {
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_COLOR_MATERIAL);
    
    GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 0.0f }; 
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.7f, 0.6f, 1.0f }; 
    GLfloat specular[] = { 0.9f, 0.9f, 0.8f, 1.0f };    

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    GLfloat gold_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
    GLfloat gold_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
    GLfloat gold_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
    GLfloat gold_shininess[] = { 51.2f }; 

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gold_shininess);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(0.0, 3.0, 5.0,  
              0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    
    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 0.5f, 0.866f); 
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glNormal3f(0.866f, 0.5f, 0.0f);  
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glNormal3f(0.0f, 0.5f, -0.866f); 
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glNormal3f(-0.866f, 0.5f, 0.0f); 
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); 
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    glutSwapBuffers();
}

void idle() {
    rotationAngle += 0.02f; 
    if (rotationAngle > 360.0f)
        rotationAngle -= 360.0f;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Gold Pyramid Lighting");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    initLighting(); 

    glutMainLoop();
    return 0;
}
