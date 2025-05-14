#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#ifndef M_PI
#define M_PI acos(-1.0)
#endif

// Draw a regular polygon
void drawPolygon(int sides, float radius, float centerX, float centerY, const std::vector<float>& color) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * i / sides;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Draw a triangle
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const std::vector<float>& color) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// Draw a line
void drawLine(float x1, float y1, float x2, float y2, const std::vector<float>& color, float width = 1.0f) {
    glColor3f(color[0], color[1], color[2]);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

// Draw a pentagon
void drawPentagon(float centerX, float centerY, float radius, float rotationAngle, const std::vector<float>& color) {
    const int numVertices = 5;
    const float angleIncrement = 2.0f * M_PI / numVertices;

    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; ++i) {
        float angle = i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

// Draw text
void drawText(float x, float y, const std::string& text, const std::vector<float>& color) {
    glColor3f(color[0], color[1], color[2]);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the red pentagon
    float pentagonRadius = 0.1f;
    float centerX = 0.5f;
    float centerY = 0.5f;
    std::vector<float> pentagonColor = { 1.0f, 0.0f, 0.0f };
    drawPentagon(centerX, centerY, pentagonRadius, -90.0f, pentagonColor);

    // Draw the yellow star points (triangles)
    float triangleHeight = 0.1f;
    std::vector<float> triangleColor = { 1.0f, 1.0f, 0.0f };

    for (int i = 0; i < 5; ++i) {
        float angle1 = 2.0f * M_PI * i / 5;
        float angle2 = 2.0f * M_PI * (i + 1) / 5;

        float x1 = centerX + pentagonRadius * cos(angle1 - M_PI / 2.0f);
        float y1 = centerY + pentagonRadius * sin(angle1 - M_PI / 2.0f);
        float x2 = centerX + pentagonRadius * cos(angle2 - M_PI / 2.0f);
        float y2 = centerY + pentagonRadius * sin(angle2 - M_PI / 2.0f);

        // Calculate triangle tip
        float tipX = centerX + (pentagonRadius + triangleHeight) * cos(angle1 + M_PI / 5 - M_PI / 2.0f);
        float tipY = centerY + (pentagonRadius + triangleHeight) * sin(angle1 + M_PI / 5 - M_PI / 2.0f);

        drawTriangle(x1, y1, x2, y2, tipX, tipY, triangleColor);
    }

    // Draw the hooks (lines and green triangles)
    float hookStartX = centerX + pentagonRadius * cos(2 * M_PI / 5 - M_PI / 2.0f);
    float hookStartY = centerY + pentagonRadius * sin(2 * M_PI / 5 - M_PI / 2.0f);
    float innerHookX = centerX + (pentagonRadius / 2.0f) * cos(2 * M_PI / 5 + M_PI / 5 - M_PI / 2.0f);
    float innerHookY = centerY + (pentagonRadius / 2.0f) * sin(2 * M_PI / 5 + M_PI / 5 - M_PI / 2.0f);
    std::vector<float> darkGreen = { 0.0f, 0.5f, 0.0f };
    std::vector<float> green = { 0.0f, 1.0f, 0.0f };

    // Draw line going right and down
    float lineY = 0.47f;
    float lineStartX = innerHookX + 0.0465f;
    drawLine(lineStartX, lineY, lineStartX + 0.2f, lineY, darkGreen, 3.0f);
    drawLine(lineStartX + 0.2f, lineY, lineStartX + 0.2f, lineY - 0.29f, darkGreen, 3.0f);

    // Draw the two "hook" triangles
    float hookTriangleBaseLength = 0.05f;
    float hookTriangleHeight = 0.1f;
    float hookEndX = lineStartX + 0.2f;
    float hookEndY = lineY - 0.2f;
    float hookEndYR = lineY - 0.1f;
    drawTriangle(hookEndX, hookEndYR - 0.04f, hookEndX + hookTriangleBaseLength, hookEndYR, hookEndX, hookEndYR - hookTriangleHeight, green);
    drawTriangle(hookEndX, hookEndY - 0.04f, hookEndX - hookTriangleBaseLength, hookEndY, hookEndX, hookEndY - hookTriangleHeight, green);

    // Draw the bucket
    float bucketX = hookEndX + 0.0f;
    float bucketY = hookEndY - 0.25f;
    float bucketTopWidth = 0.17f;
    float bucketBottomWidth = 0.13f;
    float bucketHeight = 0.15f;

    // Layer colors
    std::vector<float> layerColors[] = {
        {0.0f, 0.0f, 0.0f},
        {0.5f, 0.5f, 0.5f},
        {0.0f, 0.0f, 1.0f},
        {0.5f, 0.0f, 0.5f},
        {1.0f, 0.75f, 0.8f}
    };

    // Calculate bucket vertices
    float topLeftX = bucketX - bucketTopWidth / 2;
    float topLeftY = bucketY + bucketHeight;
    float topRightX = bucketX + bucketTopWidth / 2;
    float topRightY = bucketY + bucketHeight;
    float bottomLeftX = bucketX - bucketBottomWidth / 2;
    float bottomLeftY = bucketY;
    float bottomRightX = bucketX + bucketBottomWidth / 2;
    float bottomRightY = bucketY;

    // Draw the bucket layers
    float layerHeight = bucketHeight / 5.0f;

    for (int i = 0; i < 5; ++i) {
        float y1 = bucketY + i * layerHeight;
        float y2 = bucketY + (i + 1) * layerHeight;

        // Layer x coordinates
        float ratio1 = i * layerHeight / bucketHeight;
        float ratio2 = (i + 1) * layerHeight / bucketHeight;

        float leftX1 = bottomLeftX + (topLeftX - bottomLeftX) * ratio1;
        float rightX1 = bottomRightX + (topRightX - bottomRightX) * ratio1;
        float leftX2 = bottomLeftX + (topLeftX - bottomLeftX) * ratio2;
        float rightX2 = bottomRightX + (topRightX - bottomRightX) * ratio2;

        // Draw layer trapezoid
        glColor3f(layerColors[i][0], layerColors[i][1], layerColors[i][2]);
        glBegin(GL_QUADS);
        glVertex2f(leftX1, y1);
        glVertex2f(rightX1, y1);
        glVertex2f(rightX2, y2);
        glVertex2f(leftX2, y2);
        glEnd();
    }

    // Draw the bucket outline
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(bottomLeftX, bottomLeftY);
    glVertex2f(bottomRightX, bottomRightY);
    glVertex2f(topRightX, topRightY);
    glVertex2f(topLeftX, topLeftY);
    glEnd();

    // Draw horizontal lines between layers
    for (int i = 1; i < 5; ++i) {
        float y = bucketY + i * layerHeight;
        float ratio = i * layerHeight / bucketHeight;
        float leftX = bottomLeftX + (topLeftX - bottomLeftX) * ratio;
        float rightX = bottomRightX + (topRightX - bottomRightX) * ratio;

        glBegin(GL_LINES);
        glVertex2f(leftX, y);
        glVertex2f(rightX, y);
        glEnd();
    }
    glLineWidth(1.0f);

    // Draw the text labels inside the bucket
    std::string layerText[] = { "black", "gray", "blue", "purple", "pink" };

    for (int i = 0; i < 5; ++i) {
        float y = bucketY + (i + 0.5f) * layerHeight;
        float ratio = (i + 0.5f) * layerHeight / bucketHeight;
        float leftX = bottomLeftX + (topLeftX - bottomLeftX) * ratio;
        float rightX = bottomRightX + (topRightX - bottomRightX) * ratio;

        float midX = (leftX + rightX) / 2.0f;
        drawText(bucketX - 0.03f, y - 0.01f, layerText[i], { 1.0f, 1.0f, 1.0f });
    }

    glFlush();
}

void init() {
    glClearColor(0.7, 0.7, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Star with Hooks and Bucket");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

