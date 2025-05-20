#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    
    GLfloat material_ambient[] = { 0.0f, 0.0f, 0.3f, 1.0f };
    GLfloat material_diffuse[] = { 0.0f, 0.0f, 0.8f, 1.0f };
    GLfloat material_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat material_shininess[] = { 30.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

    
    static float angle = 0.0f;
    glRotatef(angle, 1.0f, 1.0f, 0.0f); 
    angle += 0.1f; 
    


    glColor3f(0.0f, 0.5f, 1.0f); 

    
    glutSolidCube(2.0);

    glutSwapBuffers();
    glutPostRedisplay(); 
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); 
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Lighting Demo");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); 
    initLighting();
    glutMainLoop();
    return 0;
}
