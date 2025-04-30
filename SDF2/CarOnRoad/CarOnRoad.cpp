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
const float playerSpeed = 0.1f;

struct Entity {
    float x, y, size;
    bool isCoin;
};
std::vector<Entity> entities;
float entitySpeed = 0.01f;
float spawnTimer = 0.2f;
float spawnInterval = 0.5f;

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

void drawFuelTank(float x, float y, float size) {
    // Dimensions
    float tankWidth = size * 0.8f;
    float tankHeight = size * 1.2f;
    float pipeWidth = size * 0.12f;
    float pipeLength = size * 0.6f;
    float capSize = size * 0.2f;

    // Main tank body (red gallon)
    drawRect(x - tankWidth / 2.0f, y - tankHeight / 2.0f,
        tankWidth, tankHeight, 0.8f, 0.1f, 0.1f); // bright red

    // Fuel cap (dark gray)
    float capX = x - tankWidth / 2.0f + capSize / 2.0f;
    float capY = y - tankHeight / 2.0f + capSize / 2.0f;
    drawRect(capX - capSize / 2.0f, capY - capSize / 2.0f,
        capSize, capSize, 0.3f, 0.3f, 0.3f); // darker gray cap

    // Pipe/hose on right side (green hose)
    //float pipeX = x + tankWidth / 2.0f;
    float pipeX = x - tankWidth / 2.0f;

    float pipeY = y;
    drawRect(pipeX, pipeY - pipeWidth / 2.0f,
        pipeLength, pipeWidth, 0.2f, 0.6f, 0.2f); // green pipe

    // Fuel level indicator (optional)
    float fuelLevel = 0.65f;
    float fuelBarHeight = tankHeight * fuelLevel;
    float fuelBarWidth = size * 0.06f;
    drawRect(x - tankWidth / 2.0f + fuelBarWidth,
        y + fuelBarHeight / 2.0f - tankHeight / 2.0f,
        fuelBarWidth, fuelBarHeight, 1.0f, 1.0f, 0.2f); // yellowish bar
}

void drawHighwayCone(float x, float y, float size) {
    // Radii for each layer (top-down stack)
    float baseRadius = size * 0.6f;
    float midRadius1 = size * 0.45f;
    float stripeRadius = size * 0.35f;
    float midRadius2 = size * 0.27f;
    float topRadius = size * 0.18f;
    float capRadius = size * 0.09f;

    // Base cone color
    float orangeR = 1.0f, orangeG = 0.5f, orangeB = 0.0f;

    // Bottom base (darkest orange)
    drawCircle(x, y, baseRadius, orangeR * 0.7f, orangeG * 0.4f, orangeB * 0.2f);

    // Mid layer 1 (lighter orange)
    drawCircle(x, y, midRadius1, orangeR * 0.85f, orangeG * 0.6f, orangeB * 0.3f);

    // White reflective stripe
    drawCircle(x, y, stripeRadius, 1.0f, 1.0f, 1.0f);

    // Mid layer 2 (brighter orange)
    drawCircle(x, y, midRadius2, orangeR * 0.95f, orangeG * 0.7f, orangeB * 0.4f);

    // Top layer (pure orange)
    drawCircle(x, y, topRadius, orangeR, orangeG, orangeB);

    // Dark center cap (shadow)
    drawCircle(x, y, capRadius, 0.2f, 0.1f, 0.0f);

    // Optional: small highlight to simulate gloss
    drawCircle(x + topRadius * 0.3f, y - topRadius * 0.3f, topRadius * 0.2f, 1.0f, 1.0f, 1.0f);
}

void drawObstacleBarricade(float x, float y, float size) {
    // Main body size
    float width = size * 1.5f;
    float height = size * 0.5f;
    float stripeWidth = width / 5.0f; // Five stripes

    // Background of the barricade (dark gray frame)
    //drawRect(x - width / 2.0f, y - height / 2.0f, width, height, 0.2f, 0.2f, 0.2f);

    // Draw vertical orange-white stripes
    for (int i = 0; i < 5; i++) {
        float stripeX = x - width / 2.0f + i * stripeWidth;

        // Alternate colors: orange and white
        if (i % 2 == 0) {
            drawRect(stripeX, y - height / 2.0f, stripeWidth, height, 1.0f, 0.5f, 0.0f); // orange
        }
        else {
            drawRect(stripeX, y - height / 2.0f, stripeWidth, height, 1.0f, 1.0f, 1.0f); // white
        }
    }

    // Legs (underneath the barricade)
    float legWidth = size * 0.15f;
    float legHeight = size * 0.4f;
    float legOffset = width * 0.3f;
    float legY = y + height / 2.0f + legHeight / 2.0f; // placed below the barricade

    // Left leg
    drawRect(x - legOffset - legWidth / 2.0f, legY, legWidth, legHeight, 0.3f, 0.3f, 0.3f);

    // Right leg
    drawRect(x + legOffset - legWidth / 2.0f, legY, legWidth, legHeight, 0.3f, 0.3f, 0.3f);
}


void display() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawHighwayLines();
    drawCar(playerX, playerY);

    // Main function to replace the original drawing code
    for (const auto& e : entities) {
        if (e.isCoin) {
            // Draw fuel tank instead of circle
            drawFuelTank(e.x, e.y, e.size);
        }
        else {
            // Draw highway cone instead of rectangle
            //drawHighwayCone(e.x, e.y, e.size);
            drawObstacleBarricade(e.x, e.y, e.size);
        }
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