#define NOMINMAX
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 600;

float playerX = 0.0f;
const float playerY = -0.8f;
const float playerWidth = 0.15f;
const float playerHeight = 0.08f;
const float playerSpeed = 0.08f;

struct Entity {
    float x, y, size;
    bool isCoin;
};
std::vector<Entity> entities;
float entitySpeed = 0.01f;
float spawnTimer = 0.0f;
float spawnInterval = 1.0f;

int score = 0;
int highscore = 0;
bool gameOver = false;

float randX() {
    return -0.85f + static_cast<float>(rand()) / RAND_MAX * 1.7f;
}

void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x - w / 2, y - h / 2);
    glVertex2f(x + w / 2, y - h / 2);
    glVertex2f(x + w / 2, y + h / 2);
    glVertex2f(x - w / 2, y + h / 2);
    glEnd();
}

void drawCircle(float x, float y, float radius, float cr, float cg, float cb) {
    glColor3f(cr, cg, cb);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    const int segments = 20;
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * 3.1415926535f / segments;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void resetGame() {
    entities.clear();
    playerX = 0.0f;
    score = 0;
    entitySpeed = 0.01f;
    spawnInterval = 1.0f;
    gameOver = false;
    spawnTimer = 0.0f;
}

void drawHighwayLines() {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (float y = -1.0f; y <= 1.0f; y += 0.2f) {
        drawRect(0.0f, y, 0.02f, 0.05f, 1.0f, 1.0f, 1.0f);
    }
}

void drawCar(float x, float y) {
    float bodyWidth = 0.1f;
    float bodyHeight = 0.2f;

    // Body of the car
    drawRect(x, y, bodyWidth, bodyHeight, 0.2f, 0.8f, 1.0f); // light blue body

    // Roof of the car
    drawRect(x, y + 0.03f, bodyWidth * 0.6f, bodyHeight * 0.4f, 0.1f, 0.6f, 0.9f); // darker blue roof

    // Windshield (front, top of the car)
    drawRect(x, y + 0.07f, bodyWidth * 0.5f, bodyHeight * 0.2f, 0.1f, 0.1f, 0.1f); // light windshield

    // Rear window
    drawRect(x, y - 0.06f, bodyWidth * 0.5f, bodyHeight * 0.2f, 0.1f, 0.1f, 0.1f);

    // Left wheel (bottom left)
    drawRect(x - bodyWidth * 0.6f, y - 0.05f, 0.02f, 0.05f, 0.1f, 0.1f, 0.1f);

    // Right wheel (bottom right)
    drawRect(x + bodyWidth * 0.6f, y - 0.05f, 0.02f, 0.05f, 0.1f, 0.1f, 0.1f);

    // Left wheel (top left)
    drawRect(x - bodyWidth * 0.6f, y + 0.05f, 0.02f, 0.05f, 0.1f, 0.1f, 0.1f);

    // Right wheel (top right)
    drawRect(x + bodyWidth * 0.6f, y + 0.05f, 0.02f, 0.05f, 0.1f, 0.1f, 0.1f);

    // Headlights (front top)
    drawRect(x - bodyWidth * 0.3f, y + 0.11f, 0.01f, 0.015f, 1.0f, 1.0f, 0.6f);
    drawRect(x + bodyWidth * 0.3f, y + 0.11f, 0.01f, 0.015f, 1.0f, 1.0f, 0.6f);
}


void display() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawHighwayLines();
    drawCar(playerX, playerY);

    for (const auto& e : entities) {
        if (e.isCoin)
            drawCircle(e.x, e.y, e.size / 2.0f, 1.0f, 0.9f, 0.1f);
        else
            drawRect(e.x, e.y, e.size, e.size, 1.0f, 0.2f, 0.2f);
    }

    drawText(-0.98f, 0.92f, "Score: " + std::to_string(score));
    drawText(0.5f, 0.92f, "Highscore: " + std::to_string(highscore));

    if (gameOver) {
        drawText(-0.18f, 0.0f, "GAME OVER");
        drawText(-0.28f, -0.08f, "Press R to Restart");
    }

    glutSwapBuffers();
}

void timer(int v) {
    if (!gameOver) {
        for (auto& e : entities) {
            e.y -= entitySpeed;
        }

        entities.erase(
            std::remove_if(entities.begin(), entities.end(), [](const Entity& e) {
                return e.y < -1.1f;
                }),
            entities.end()
        );

        spawnTimer += 0.016f;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            Entity newEntity;
            newEntity.x = randX();
            newEntity.y = 1.1f;
            newEntity.size = 0.07f + (rand() % 2) * 0.03f;
            newEntity.isCoin = (rand() % 3 != 0);
            entities.push_back(newEntity);
        }

        for (auto it = entities.begin(); it != entities.end();) {
            bool removed = false;
            float entityHalfSize = it->size / 2.0f;
            float playerHalfWidth = playerWidth / 2.0f;
            float playerHalfHeight = playerHeight / 2.0f;

            if (fabs(it->x - playerX) < (playerHalfWidth + entityHalfSize) &&
                fabs(it->y - playerY) < (playerHalfHeight + entityHalfSize)) {
                if (it->isCoin) {
                    score++;
                    if (score > highscore) {
                        highscore = score;
                    }
                    it = entities.erase(it);
                    removed = true;
                }
                else {
                    gameOver = true;
                    break;
                }
            }

            if (!removed && !gameOver) {
                ++it;
            }
            else if (gameOver) {
                break;
            }
        }

        if (score > 0 && score % 10 == 0) {
            entitySpeed = 0.012f + (score / 10) * 0.001f;
            spawnInterval = std::max(0.4f, 1.0f - (score / 10) * 0.05f);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (gameOver && (key == 'r' || key == 'R')) {
        resetGame();
    }
    if (key == 27) {
        glutLeaveMainLoop();
    }
}

void special(int key, int x, int y) {
    if (!gameOver) {
        if (key == GLUT_KEY_LEFT && playerX - playerWidth / 2 > -1.0f) {
            playerX -= playerSpeed;
        }
        if (key == GLUT_KEY_RIGHT && playerX + playerWidth / 2 < 1.0f) {
            playerX += playerSpeed;
        }
        playerX = std::max(-1.0f + playerWidth / 2, std::min(1.0f - playerWidth / 2, playerX));
    }
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(0)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Simple Coin Collector");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(16, timer, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    resetGame();
    glutMainLoop();

    return 0;
}