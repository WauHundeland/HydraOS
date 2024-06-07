#include "App.h"
#include "M5Cardputer.h"
#include "SD.h"
#include "ScreenManager.h"
#include "Utils.h"
#include "AppManager.h"
#include "icon.h"

#pragma once

class DinoObstacle {
public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    bool isPassed;
    bool isCollided;
    DinoObstacle(int x, int y, int width, int height, int speed) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed = speed;
        isPassed = false;
        isCollided = false;
    }
};

class DinoRunner : public App {
private:
    int score;
    bool isGameOver;
    float downForce;
    float jumpForce;
    float gravity;
    float dinoX;
    float dinoY;
    float dinoWidth;
    float dinoHeight;
    std::vector<DinoObstacle> obstacles;
    int obstacleSpawnTimer;

public:
    const char *getName() override {
        return "DinoRunner";
    }

    void onAppClose() override {
        // Clean up the game here
        obstacles.clear();
        isGameOver = false;
    }

    void onAppOpen() override {
        // Initialize the game here
        obstacles.clear();
        score = 0;
        isGameOver = false;
        // Initialize the dinosaur character and obstacles here
        downForce = 0;
        jumpForce = 0;
        gravity = 0.5;
        dinoX = 20;
        dinoY = 100;
        dinoWidth = 10;
        dinoHeight = 10;
        obstacles.emplace_back(220, 100, 10, 10, 2);
        obstacleSpawnTimer = 70;
    }

    void onAppTick() override {
        // Update the game state here
        if (!isGameOver) {
            // Update the dinosaur character and obstacles here
            // Check for collisions between the dinosaur and obstacles
            // If a collision is detected, set isGameOver to true
            // draw the dino now
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.fillRect(dinoX, dinoY, dinoWidth, dinoHeight, RED);
            canvas.drawFastHLine(5, 110, 230, WHITE);
            // draw the obstacles
            for (DinoObstacle &obstacle : obstacles) {
                // delete the obstacle if it is out of the screen
                if (obstacle.x < 0) {
                    obstacles.erase(obstacles.begin());
                }
                canvas.fillRect(obstacle.x, obstacle.y, obstacle.width, obstacle.height, GREEN);
                obstacle.x -= obstacle.speed;
                if (obstacle.x < dinoX + dinoWidth && obstacle.x + obstacle.width > dinoX && obstacle.y < dinoY + dinoHeight && obstacle.y + obstacle.height > dinoY) {
                    isGameOver = true;
                }
                if (obstacle.x < (dinoX+10) && !obstacle.isPassed) {
                    score++;
                    obstacle.isPassed = true;
                    // add a new obstacle with random properties
                }
            }
            // make the dino jump if a key is pressed
            if (M5Cardputer.Keyboard.isPressed() && jumpForce == 0 && dinoY == 100) {
                jumpForce = 7;
            }
            // apply gravity to the dino if it is not on the ground and not jumping
            if (dinoY < 100 && jumpForce == 0) {
                downForce += gravity;
                dinoY += downForce;
            } else {
                downForce = 0;
                if (dinoY > 100) {
                    dinoY = 100;
                }
            }
            // apply jump force
            if (jumpForce > 0) {
                dinoY -= jumpForce;
                jumpForce -= 0.5; // decrease jump force over time
            }
            // decrease the spawn timer
            obstacleSpawnTimer--;

            // if the spawn timer reaches 0, spawn a new obstacle
            if (obstacleSpawnTimer <= 0) {
                int randomHeight = random(10, 20);
                int randomWidth = random(5, 20);
                int randomSpeed = random(2, 3);
                obstacles.emplace_back(230-randomWidth, 100 - randomHeight + 10, randomWidth, randomHeight, randomSpeed);
                // reset the spawn timer to a random value
                obstacleSpawnTimer = random(30, 65);
            }
        }
    }

    void draw() override {
        // Draw the game on the screen here
        // Draw the dinosaur character and obstacles
        // If isGameOver is true, display a game over message
        if (isGameOver) {
            Utils::popup("Game Over! Score: " + String(score), 0, 1.5);
            AppManager::getInstance().closeCurrentApp();
            AppManager::getInstance().openApp("DinoRunner");
        } else {
            canvas.pushSprite(0, 0);
        }
    }

    const uint8_t *getIcon() override {
        return dino_icon;
    }

    int getIconSize() override {
        return sizeof(dino_icon);
    }
};