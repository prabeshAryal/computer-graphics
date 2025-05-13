#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

//#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <algorithm>


using namespace std;

// ==================== CONSTANTS ====================
const int windowWidth = 800, windowHeight = 600;
const int playerWidth = 26, playerHeight = 30;
const int playerInitialHealth = 3, playerMaxHealth = 5;
const int playerMoveSpeed = 30;
const float invincibilityDuration = 1.0f;
const int blockHeight = 30;
const int numberOfGaps = 3, numberOfSegments = numberOfGaps + 1;
const int minSegmentWidth = 40;
const int initialMinGapWidth = 70, absoluteMinGapWidth = 40;
const int gapDecreasePerLevel = 4;
const float initialBlockSeparationFactor = 1.1f, minBlockSeparationFactor = 0.65f;
const float separationDecreasePerLevel = 0.03f;
const int itemSize = 18;
const float itemSpawnProbability = 0.045f;
const int itemScoreBonus = 15;
const float itemMaxHorizontalSpeedFactor = 0.4f;
const float initialBlockSpeed = 2.5f, speedIncreasePerLevel = 0.25f;
const int speedIncreaseInterval = 2;
const float maxSpeedMultiplier = 4.5f;
const int numberOfStars = 100;
const float PI = 3.14159265f;

// ==================== ENUMS & STRUCTS ====================
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };
enum ItemType { SCORE_POWERUP, HEALTH_POWERUP, OBSTACLE };

struct Block {
    int yPosition;
    int segmentStartX[numberOfSegments], segmentEndX[numberOfSegments];
    bool hasBeenPassedByPlayer;
    bool isActive;
};

struct Item {
    int xPosition;
    int yPosition;
    int itemSize;
    ItemType itemType;
    float horizontalSpeed;
    float verticalSpeed;
    bool isActive;
};

struct Star {
    float xPosition;
    float yPosition;
    float speed;
    float itemSize;
};

// ==================== GLOBAL VARIABLES ====================
GameState gameState = MENU;
int playerX = windowWidth / 2, playerY = 50, playerHealth = playerInitialHealth;
int score = 0, blocksPassedCount = 0;
float currentBlockSpeed = initialBlockSpeed;
float currentBlockSeparation = windowHeight * initialBlockSeparationFactor;
int currentMinGapWidth = initialMinGapWidth;
bool isPlayerInvincible = false;
float invincibilityTimer = 0.0f;

const int maxBlocks = 8;
Block activeBlocks[maxBlocks];
int numberOfActiveBlocks = 0;

const int maxItems = 20;
Item activeItems[maxItems];
int numberOfActiveItems = 0;

Star backgroundStars[numberOfStars];


bool checkCollision(int playerLeft, int playerBottom, int playerWidthParam, int playerHeightParam,
    int objectLeft, int objectBottom, int objectWidth, int objectHeight) {
    return playerLeft < objectLeft + objectWidth &&
        playerLeft + playerWidthParam > objectLeft &&
        playerBottom < objectBottom + objectHeight &&
        playerBottom + playerHeightParam > objectBottom;
}

void drawText(float x, float y, const char* textToDraw, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (const char* character = textToDraw; *character != '\0'; character++)
        glutBitmapCharacter(font, *character);
}

void drawTextCentered(float y, const char* textToDraw, void* font = GLUT_BITMAP_HELVETICA_18) {
    int textWidth = 0;
    for (const char* character = textToDraw; *character != '\0'; ++character)
        textWidth += glutBitmapWidth(font, *character);
    drawText(windowWidth / 2.0f - textWidth / 2.0f, y, textToDraw, font);
}

void drawOverlay(float red, float green, float blue, float alpha) {
    glColor4f(red, green, blue, alpha);
    glBegin(GL_QUADS);
    glVertex2i(0, 0); glVertex2i(windowWidth, 0);
    glVertex2i(windowWidth, windowHeight); glVertex2i(0, windowHeight);
    glEnd();
}

void drawHeartShape(int centerX, int centerY, int halfSize) {
    glBegin(GL_TRIANGLES);
    glVertex2i(centerX - halfSize, centerY);
    glVertex2i(centerX + halfSize, centerY);
    glVertex2i(centerX, centerY - (int)(halfSize * 1.1f));
    glEnd();

    glBegin(GL_POLYGON);
    for (int j = 0; j <= 10; ++j) {
        float angle = PI * j / 10.f;
        glVertex2f(centerX - halfSize / 2.f + cos(angle) * halfSize / 1.5f, centerY + sin(angle) * halfSize / 1.5f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int j = 0; j <= 10; ++j) {
        float angle = PI * j / 10.f;
        glVertex2f(centerX + halfSize / 2.f + cos(angle) * halfSize / 1.5f, centerY + sin(angle) * halfSize / 1.5f);
    }
    glEnd();
}

void handlePlayerHit() {
    if (isPlayerInvincible) return;
    playerHealth--;
    isPlayerInvincible = true;
    invincibilityTimer = invincibilityDuration;
    if (playerHealth <= 0)
        gameState = GAME_OVER;
}

void compactBlocksArray(Block arrayToCompact[], int& elementCount) {
    int writeIndex = 0;
    for (int readIndex = 0; readIndex < elementCount; ++readIndex)
        if (arrayToCompact[readIndex].isActive) {
            if (readIndex != writeIndex)
                arrayToCompact[writeIndex] = arrayToCompact[readIndex];
            writeIndex++;
        }
    elementCount = writeIndex;
}

void compactItemsArray(Item arrayToCompact[], int& elementCount) {
    int writeIndex = 0;
    for (int readIndex = 0; readIndex < elementCount; ++readIndex)
        if (arrayToCompact[readIndex].isActive) {
            if (readIndex != writeIndex)
                arrayToCompact[writeIndex] = arrayToCompact[readIndex];
            writeIndex++;
        }
    elementCount = writeIndex;
}


void clampPlayerPosition() {
    playerX = max(playerWidth / 2, min(playerX, windowWidth - playerWidth / 2));
}


// ==================== DRAWING FUNCTIONS ====================
void drawGameObjects() {
    glColor3f(0.8f, 0.8f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < numberOfStars; ++i) {
        glPointSize(backgroundStars[i].itemSize);
        glVertex2f(backgroundStars[i].xPosition, backgroundStars[i].yPosition);
    }
    glEnd();
    glPointSize(1.0f);

    glColor3f(0.9f, 0.25f, 0.2f);
    for (int i = 0; i < numberOfActiveBlocks; ++i)
        if (activeBlocks[i].isActive)
            for (int j = 0; j < numberOfSegments; ++j) {
                glBegin(GL_QUADS);
                glVertex2i(activeBlocks[i].segmentStartX[j], activeBlocks[i].yPosition);
                glVertex2i(activeBlocks[i].segmentEndX[j], activeBlocks[i].yPosition);
                glVertex2i(activeBlocks[i].segmentEndX[j], activeBlocks[i].yPosition + blockHeight);
                glVertex2i(activeBlocks[i].segmentStartX[j], activeBlocks[i].yPosition + blockHeight);
                glEnd();
            }

    for (int i = 0; i < numberOfActiveItems; ++i)
        if (activeItems[i].isActive) {
            int itemX = activeItems[i].xPosition, itemY = activeItems[i].yPosition;
            int currentItemSize = activeItems[i].itemSize, halfItemSize = currentItemSize / 2;
            switch (activeItems[i].itemType) {
            case SCORE_POWERUP:
                glColor3f(1.f, 1.f, 0.f);
                glBegin(GL_POLYGON);
                glVertex2i(itemX, itemY + halfItemSize); glVertex2i(itemX + halfItemSize, itemY);
                glVertex2i(itemX, itemY - halfItemSize); glVertex2i(itemX - halfItemSize, itemY);
                glEnd();
                break;

            case HEALTH_POWERUP:
                glColor3f(0.1f, 0.9f, 0.9f);
                drawHeartShape(itemX, itemY, halfItemSize);
                break;

            case OBSTACLE:
                glColor3f(.8f, 0.f, .8f);
                glBegin(GL_POLYGON);
                for (int j = 0; j < 8; ++j) {
                    float angle = 2.f * PI * j / 8.f;
                    glVertex2f(itemX + cos(angle) * halfItemSize * .7f, itemY + sin(angle) * halfItemSize * .7f);
                }
                glEnd();
                glLineWidth(2.f);
                glBegin(GL_LINES);
                for (int j = 0; j < 8; ++j) {
                    float angle = 2.f * PI * j / 8.f;
                    glVertex2f(itemX + cos(angle) * halfItemSize * .6f, itemY + sin(angle) * halfItemSize * .6f);
                    glVertex2f(itemX + cos(angle) * halfItemSize * 1.1f, itemY + sin(angle) * halfItemSize * 1.1f);
                }
                glEnd();
                glLineWidth(1.f);
                break;
            }
        }

    if (!isPlayerInvincible || fmod(invincibilityTimer, 0.2f) < 0.1f) {
        float healthRatio = max(0.0f, (float)playerHealth / playerInitialHealth);
        glColor3f(min(1.0f, (1.0f - healthRatio) * 1.5f), min(1.0f, healthRatio * 1.2f), 0.1f);
        glBegin(GL_TRIANGLES);
        glVertex2i(playerX, playerY + playerHeight);
        glVertex2i(playerX - playerWidth / 2, playerY);
        glVertex2i(playerX + playerWidth / 2, playerY);
        glEnd();

        glColor3f(0.8f, 0.8f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2i(playerX - playerWidth / 4, playerY);
        glVertex2i(playerX + playerWidth / 4, playerY);
        glVertex2i(playerX, playerY - 5);
        glEnd();
    }
}

void drawUI() {
    char uiTextBuffer[50];
    snprintf(uiTextBuffer, 50, "Score: %d", score);
    glColor3f(1.f, 1.f, 1.f);
    drawText(10, windowHeight - 30, uiTextBuffer);

    int heartSize = 18, heartSpacing = 6;
    int startX = windowWidth - (playerMaxHealth * (heartSize + heartSpacing)) + heartSpacing / 2;
    int heartY = windowHeight - heartSize - 15;
    for (int i = 0; i < playerMaxHealth; ++i) {
        bool hasHealthPoint = (i < playerHealth);
        glColor3f(hasHealthPoint ? 1.f : .5f, hasHealthPoint ? 0.2f : .5f, hasHealthPoint ? 0.2f : .5f);
        drawHeartShape(startX + i * (heartSize + heartSpacing), heartY, heartSize / 2);
    }
}

// ==================== SPAWNING BLOCK ====================
void spawnBlock() {
    if (numberOfActiveBlocks >= maxBlocks || numberOfGaps <= 0) return;

    Block& newBlock = activeBlocks[numberOfActiveBlocks];
    int effectiveMinGapWidth = max(absoluteMinGapWidth, currentMinGapWidth);
    int totalMinimumWidthRequired = numberOfSegments * minSegmentWidth + numberOfGaps * effectiveMinGapWidth;

    if (totalMinimumWidthRequired > windowWidth)
        effectiveMinGapWidth = max(1, effectiveMinGapWidth - (totalMinimumWidthRequired - windowWidth + numberOfGaps - 1) / numberOfGaps);

    int availableWidthForDistribution = max(0, windowWidth - (numberOfSegments * minSegmentWidth + numberOfGaps * effectiveMinGapWidth));
    int totalExtraGapWidth = (availableWidthForDistribution > 0) ? (rand() % (availableWidthForDistribution + 1)) : 0;
    int totalExtraSegmentWidth = availableWidthForDistribution - totalExtraGapWidth;

    int gapSizes[numberOfGaps];
    int segmentSizes[numberOfSegments];

    for (int i = 0; i < numberOfGaps; ++i) gapSizes[i] = effectiveMinGapWidth;
    for (int i = 0; i < numberOfSegments; ++i) segmentSizes[i] = minSegmentWidth;


    if (totalExtraGapWidth > 0 && numberOfGaps > 0) {
        for (int i = 0; i < totalExtraGapWidth; ++i)
            gapSizes[rand() % numberOfGaps]++;
    }

    if (totalExtraSegmentWidth > 0 && numberOfSegments > 0) {
        for (int i = 0; i < totalExtraSegmentWidth; ++i)
            segmentSizes[rand() % numberOfSegments]++;
    }

    int totalCalculatedWidth = 0;
    for (int i = 0; i < numberOfSegments; ++i) totalCalculatedWidth += segmentSizes[i];
    for (int i = 0; i < numberOfGaps; ++i) totalCalculatedWidth += gapSizes[i];
    int widthDifference = windowWidth - totalCalculatedWidth;

    if (widthDifference != 0) {
        int segmentIndices[numberOfSegments];
        for (int i = 0; i < numberOfSegments; ++i) segmentIndices[i] = i;
        for (int i = 0; i < numberOfSegments - 1; ++i) {
            for (int j = 0; j < numberOfSegments - i - 1; ++j) {
                if (segmentSizes[segmentIndices[j]] < segmentSizes[segmentIndices[j + 1]]) {
                    int temp = segmentIndices[j];
                    segmentIndices[j] = segmentIndices[j + 1];
                    segmentIndices[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < abs(widthDifference); ++i) {
            bool adjusted = false;
            int currentSegmentIndex = segmentIndices[i % numberOfSegments];
            if (widthDifference > 0) {
                segmentSizes[currentSegmentIndex]++;
                adjusted = true;
            }
            else {
                if (segmentSizes[currentSegmentIndex] > minSegmentWidth) {
                    segmentSizes[currentSegmentIndex]--;
                    adjusted = true;
                }
            }
            if (!adjusted && widthDifference < 0) break;
        }
    }

    int requiredMinimumGapSizeForPlayer = playerWidth + 10;
    int largestGapIndex = -1;
    int largestGapSize = -1;
    for (int i = 0; i < numberOfGaps; ++i) {
        if (gapSizes[i] > largestGapSize) {
            largestGapSize = gapSizes[i];
            largestGapIndex = i;
        }
    }

    if (largestGapIndex != -1 && largestGapSize < requiredMinimumGapSizeForPlayer) {
        int neededWidthForLargestGap = requiredMinimumGapSizeForPlayer - largestGapSize;

        int segmentIndices[numberOfSegments];
        for (int i = 0; i < numberOfSegments; ++i) segmentIndices[i] = i;
        for (int i = 0; i < numberOfSegments - 1; ++i) {
            for (int j = 0; j < numberOfSegments - i - 1; ++j) {
                if (segmentSizes[segmentIndices[j]] < segmentSizes[segmentIndices[j + 1]]) {
                    int temp = segmentIndices[j];
                    segmentIndices[j] = segmentIndices[j + 1];
                    segmentIndices[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < numberOfSegments && neededWidthForLargestGap > 0; ++i) {
            int segmentIndex = segmentIndices[i];
            int widthAvailableToSteal = segmentSizes[segmentIndex] - minSegmentWidth;
            int widthActuallyStolen = min(neededWidthForLargestGap, widthAvailableToSteal);

            if (widthActuallyStolen > 0) {
                segmentSizes[segmentIndex] -= widthActuallyStolen;
                gapSizes[largestGapIndex] += widthActuallyStolen;
                neededWidthForLargestGap -= widthActuallyStolen;
            }
        }
        largestGapSize = gapSizes[largestGapIndex];
    }


    int elementTypeSequence[numberOfGaps + numberOfSegments];
    for (int i = 0; i < numberOfGaps; ++i) elementTypeSequence[i] = 0;
    for (int i = numberOfGaps; i < numberOfGaps + numberOfSegments; ++i) elementTypeSequence[i] = 1;

    int n = numberOfGaps + numberOfSegments;
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = elementTypeSequence[i];
        elementTypeSequence[i] = elementTypeSequence[j];
        elementTypeSequence[j] = temp;
    }


    int currentXPosition = 0;
    int gapIndexCounter = 0;
    int segmentIndexCounter = 0;
    for (int i = 0; i < n; ++i) {
        int elementType = elementTypeSequence[i];
        if (elementType == 0) {
            currentXPosition += (gapIndexCounter < numberOfGaps) ? max(1, gapSizes[gapIndexCounter++]) : 0;
        }
        else {
            if (segmentIndexCounter < numberOfSegments) {
                newBlock.segmentStartX[segmentIndexCounter] = currentXPosition;
                currentXPosition += segmentSizes[segmentIndexCounter];
                newBlock.segmentEndX[segmentIndexCounter] = currentXPosition;
                segmentIndexCounter++;
            }
        }
    }


    if (segmentIndexCounter > 0 && currentXPosition < windowWidth) {
        newBlock.segmentEndX[segmentIndexCounter - 1] = windowWidth;
    }


    int yOffset = rand() % 11;
    newBlock.yPosition = windowHeight + yOffset;
    newBlock.hasBeenPassedByPlayer = false;
    newBlock.isActive = true;
    numberOfActiveBlocks++;
}

// ==================== SPAWN ITEM ====================
void spawnItem() {
    if (numberOfActiveItems >= maxItems) return;

    Item& newItem = activeItems[numberOfActiveItems];
    newItem.isActive = true;
    newItem.itemSize = itemSize;
    int halfSize = newItem.itemSize / 2;

    newItem.xPosition = halfSize + (rand() % (windowWidth - 2 * halfSize + 1));

    int yOffset = rand() % 51;
    newItem.yPosition = windowHeight + yOffset;

    float speedFactor = 0.8f + ((float)rand() / RAND_MAX) * 0.5f;
    newItem.verticalSpeed = currentBlockSpeed * speedFactor;

    float currentSpeedRatio = currentBlockSpeed / initialBlockSpeed;
    float normalizedSpeedRatioFactor = (maxSpeedMultiplier > 1.f) ? max(0.f, min(1.f, (currentSpeedRatio - 1.f) / (maxSpeedMultiplier - 1.f))) : 0.f;
    float maxHorizontalSpeed = itemMaxHorizontalSpeedFactor * sqrt(normalizedSpeedRatioFactor) * currentBlockSpeed;

    if (maxHorizontalSpeed > 0.001f) {
        newItem.horizontalSpeed = -maxHorizontalSpeed + ((float)rand() / RAND_MAX) * (2.0f * maxHorizontalSpeed);
    }
    else {
        newItem.horizontalSpeed = 0.0f;
    }

    int itemTypeRoll = rand() % 100;
    if (itemTypeRoll < 15 && playerHealth < playerMaxHealth) {
        newItem.itemType = HEALTH_POWERUP;
    }
    else if (itemTypeRoll < 70) {
        newItem.itemType = SCORE_POWERUP;
    }
    else {
        newItem.itemType = OBSTACLE;
    }
    numberOfActiveItems++;
}

// ==================== UPDATE FUNCTIONS ====================
void updateGame(float deltaTime) {
    float starSpeedFactor = currentBlockSpeed / initialBlockSpeed * 60.f * deltaTime;
    for (int i = 0; i < numberOfStars; ++i) {
        backgroundStars[i].yPosition -= backgroundStars[i].speed * starSpeedFactor;
        if (backgroundStars[i].yPosition < 0) {
            backgroundStars[i].yPosition += windowHeight;
            backgroundStars[i].xPosition = ((float)rand() / RAND_MAX) * windowWidth;
        }
    }

    if (gameState != PLAYING) return;

    if (isPlayerInvincible) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0.f)
            isPlayerInvincible = false;
    }

    bool needsToSpawnBlock = (numberOfActiveBlocks == 0);
    if (!needsToSpawnBlock && numberOfActiveBlocks > 0) {
        int highestBlockY = -1;
        for (int i = 0; i < numberOfActiveBlocks; ++i)
            if (activeBlocks[i].isActive)
                highestBlockY = max(highestBlockY, activeBlocks[i].yPosition);
        if (highestBlockY != -1 && highestBlockY < windowHeight - currentBlockSeparation)
            needsToSpawnBlock = true;
    }
    if (needsToSpawnBlock && numberOfActiveBlocks < maxBlocks)
        spawnBlock();

    float pixelsToMoveDown = currentBlockSpeed * 60.f * deltaTime;
    for (int i = 0; i < numberOfActiveBlocks; ++i)
        if (activeBlocks[i].isActive) {
            activeBlocks[i].yPosition -= (int)(round(pixelsToMoveDown));

            if (!activeBlocks[i].hasBeenPassedByPlayer && (activeBlocks[i].yPosition + blockHeight < playerY)) {
                activeBlocks[i].hasBeenPassedByPlayer = true;
                score++;
                blocksPassedCount++;

                if (blocksPassedCount > 0 && blocksPassedCount % speedIncreaseInterval == 0) {
                    int difficultyLevel = blocksPassedCount / speedIncreaseInterval;
                    currentBlockSpeed = min(initialBlockSpeed + difficultyLevel * speedIncreasePerLevel, initialBlockSpeed * maxSpeedMultiplier);
                    currentMinGapWidth = max(initialMinGapWidth - difficultyLevel * gapDecreasePerLevel, absoluteMinGapWidth);
                    currentBlockSeparation = windowHeight * max(initialBlockSeparationFactor - difficultyLevel * separationDecreasePerLevel, minBlockSeparationFactor);
                }
            }

            if (activeBlocks[i].yPosition + blockHeight < 0)
                activeBlocks[i].isActive = false;
        }
    compactBlocksArray(activeBlocks, numberOfActiveBlocks);

    if (((float)rand() / RAND_MAX) < itemSpawnProbability * deltaTime * 60.f && numberOfActiveItems < maxItems)
        spawnItem();

    float timeFactor = 60.f * deltaTime;
    for (int i = 0; i < numberOfActiveItems; ++i)
        if (activeItems[i].isActive) {
            activeItems[i].yPosition -= (int)(round(activeItems[i].verticalSpeed * timeFactor));
            activeItems[i].xPosition += (int)(round(activeItems[i].horizontalSpeed * timeFactor));

            int halfItemSize = activeItems[i].itemSize / 2;
            if (activeItems[i].xPosition <= halfItemSize) {
                activeItems[i].xPosition = halfItemSize;
                if (activeItems[i].horizontalSpeed < 0)
                    activeItems[i].horizontalSpeed *= -0.8f;
            }
            else if (activeItems[i].xPosition >= windowWidth - halfItemSize) {
                activeItems[i].xPosition = windowWidth - halfItemSize;
                if (activeItems[i].horizontalSpeed > 0)
                    activeItems[i].horizontalSpeed *= -0.8f;
            }

            if (activeItems[i].yPosition + halfItemSize < 0)
                activeItems[i].isActive = false;
        }
    compactItemsArray(activeItems, numberOfActiveItems);

    if (!isPlayerInvincible) {
        int playerLeft = playerX - playerWidth / 2;
        int playerBottom = playerY;

        for (int i = 0; i < numberOfActiveBlocks && !isPlayerInvincible; ++i)
            if (activeBlocks[i].isActive &&
                playerBottom < activeBlocks[i].yPosition + blockHeight &&
                playerBottom + playerHeight > activeBlocks[i].yPosition) {
                for (int j = 0; j < numberOfSegments; ++j) {
                    if (checkCollision(playerLeft, playerBottom, playerWidth, playerHeight,
                        activeBlocks[i].segmentStartX[j], activeBlocks[i].yPosition,
                        activeBlocks[i].segmentEndX[j] - activeBlocks[i].segmentStartX[j], blockHeight)) {
                        handlePlayerHit();
                        break;
                    }
                }
            }

        for (int i = 0; i < numberOfActiveItems && !isPlayerInvincible; ++i)
            if (activeItems[i].isActive) {
                int itemSizeLocal = activeItems[i].itemSize;
                int itemLeft = activeItems[i].xPosition - itemSizeLocal / 2;
                int itemBottom = activeItems[i].yPosition - itemSizeLocal / 2;
                if (checkCollision(playerLeft, playerBottom, playerWidth, playerHeight,
                    itemLeft, itemBottom, itemSizeLocal, itemSizeLocal)) {
                    switch (activeItems[i].itemType) {
                    case SCORE_POWERUP:
                        score += itemScoreBonus;
                        break;
                    case HEALTH_POWERUP:
                        playerHealth = min(playerHealth + 1, playerMaxHealth);
                        break;
                    case OBSTACLE:
                        handlePlayerHit();
                        break;
                    }
                    activeItems[i].isActive = false;
                }
            }
    }
}

// ==================== CORE FUNCTIONS ====================
void resetGame() {
    playerX = windowWidth / 2;
    playerY = 50;
    playerHealth = playerInitialHealth;
    isPlayerInvincible = false;
    invincibilityTimer = 0.f;
    score = 0;
    blocksPassedCount = 0;
    currentBlockSpeed = initialBlockSpeed;
    currentMinGapWidth = initialMinGapWidth;
    currentBlockSeparation = windowHeight * initialBlockSeparationFactor;
    numberOfActiveBlocks = 0;
    numberOfActiveItems = 0;

    for (int i = 0; i < maxBlocks; ++i)
        activeBlocks[i].isActive = false;
    for (int i = 0; i < maxItems; ++i)
        activeItems[i].isActive = false;
}

void initStars() {
    for (int i = 0; i < numberOfStars; ++i) {
        backgroundStars[i].xPosition = (float)rand() / RAND_MAX * windowWidth;
        backgroundStars[i].yPosition = (float)rand() / RAND_MAX * windowHeight;
        float randomFactor = (float)rand() / RAND_MAX;
        backgroundStars[i].speed = 0.5f + randomFactor * 1.5f;
        backgroundStars[i].itemSize = 1.f + randomFactor * 1.5f;
    }
}

void initGL() {
    glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
    srand(time(nullptr));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    initStars();
    gameState = MENU;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawGameObjects();

    if (gameState == MENU) {
        glColor3f(1.f, .9f, .5f);
        drawTextCentered(windowHeight * .75f, "Tunnel Game", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(.9f, .9f, .9f);
        drawTextCentered(windowHeight * .6f, "A/D or Left/Right: Move", GLUT_BITMAP_HELVETICA_18);
        drawTextCentered(windowHeight * .55f, "P: Pause/Resume", GLUT_BITMAP_HELVETICA_18);
        glColor3f(.7f, 1.f, .7f);
        drawTextCentered(windowHeight * .15f, "Press S to Start", GLUT_BITMAP_HELVETICA_18);
        glColor3f(.8f, .8f, .8f);
        drawTextCentered(windowHeight * .05f, "Press ESC to Exit", GLUT_BITMAP_HELVETICA_12);
    }
    else {
        drawUI();

        if (gameState == PAUSED) {
            drawOverlay(.2f, .2f, .3f, .75f);
            glColor3f(.9f, .9f, 1.f);
            drawTextCentered(windowHeight * .55f, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
            drawTextCentered(windowHeight * .45f, "Press P to continue", GLUT_BITMAP_HELVETICA_18);
        }
        else if (gameState == GAME_OVER) {
            drawOverlay(.1f, 0.f, 0.f, .85f);
            glColor3f(1.f, .2f, .2f);
            drawTextCentered(windowHeight * .6f, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

            char scoreTextBuffer[50];
            snprintf(scoreTextBuffer, 50, "Final Score: %d", score);
            glColor3f(1.f, 1.f, 1.f);
            drawTextCentered(windowHeight * .5f, scoreTextBuffer, GLUT_BITMAP_HELVETICA_18);

            glColor3f(.9f, .9f, 1.f);
            drawTextCentered(windowHeight * .35f, "Press R to Menu", GLUT_BITMAP_HELVETICA_12);
            drawTextCentered(windowHeight * .25f, "Press ESC to Exit", GLUT_BITMAP_HELVETICA_12);
        }
    }
    glutSwapBuffers();
}

void updateCallback(int value) {
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = min((float)(currentTime - lastTime) / 1000.f, 0.05f);
    lastTime = currentTime;

    updateGame(deltaTime);
    glutPostRedisplay();
    glutTimerFunc(16, updateCallback, 0);
}

void keyboardInput(unsigned char key, int x, int y) {
    if (key >= 'A' && key <= 'Z') key = key + ('a' - 'A');

    switch (gameState) {
    case MENU:
        if (key == 's') {
            resetGame();
            gameState = PLAYING;
        }
        else if (key == 27)
            glutLeaveMainLoop();
        break;

    case PLAYING:
        if (key == 'a')
            playerX -= playerMoveSpeed;
        else if (key == 'd')
            playerX += playerMoveSpeed;
        else if (key == 'p')
            gameState = PAUSED;
        else if (key == 27)
            gameState = MENU;
        break;

    case PAUSED:
        if (key == 'p')
            gameState = PLAYING;
        else if (key == 27)
            gameState = MENU;
        break;

    case GAME_OVER:
        if (key == 'r')
            gameState = MENU;
        else if (key == 27)
            glutLeaveMainLoop();
        break;
    }
    if ((gameState == PLAYING && (key == 'a' || key == 'd')) || gameState == PAUSED || gameState == GAME_OVER) {
        clampPlayerPosition();
    }
}


void specialKeyboardInput(int key, int x, int y) {
    if (gameState == PLAYING) {
        if (key == GLUT_KEY_LEFT)
            playerX -= playerMoveSpeed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += playerMoveSpeed;
        clampPlayerPosition();
    }
}

void reshape(int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, (newHeight == 0) ? 1 : newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// ==================== MAIN FUNCTION ====================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tunnel Game");

    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardInput);
    glutSpecialFunc(specialKeyboardInput);
    glutTimerFunc(0, updateCallback, 0);
    glutMainLoop();
    return 0;
}