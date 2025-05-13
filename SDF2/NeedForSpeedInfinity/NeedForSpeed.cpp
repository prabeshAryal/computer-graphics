#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint roadTextureId;

// Function to create a simple road texture
void createRoadTexture() {
    const int textureWidth = 256;
    const int textureHeight = 256;
    unsigned char* textureData = new unsigned char[textureWidth * textureHeight * 3];

    // Create basic road texture pattern (just asphalt)
    for (int y = 0; y < textureHeight; y++) {
        for (int x = 0; x < textureWidth; x++) {
            int index = (y * textureWidth + x) * 3;

            // Base dark gray with variations
            unsigned char gray = 50 + (rand() % 15); // Dark gray base

            // Add occasional lighter gray speckles for asphalt texture
            if (rand() % 30 == 0) {
                gray = 80 + (rand() % 20); // Lighter gray bits
            }

            // Add occasional white-ish reflective specks
            if (rand() % 100 == 0) {
                gray = 180 + (rand() % 50); // Shiny bits
            }

            // Set pixel color (gray value for all channels)
            textureData[index] = gray;     // R
            textureData[index + 1] = gray; // G
            textureData[index + 2] = gray; // B
        }
    }

    // Generate texture
    glGenTextures(1, &roadTextureId);
    glBindTexture(GL_TEXTURE_2D, roadTextureId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

    delete[] textureData;
}

// Function to render the road texture
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTextureId);

    // Draw a textured quad covering the screen
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(3.0f, 3.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 3.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

// Function to initialize OpenGL
void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize random seed for texture noise
    srand(time(NULL));

    // Create the road texture
    createRoadTexture();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Road Texture - OpenGL");

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    initialize();
    glutDisplayFunc(render);
    glutMainLoop();

    return 0;
}