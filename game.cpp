#include <GL/freeglut.h>
#include <stdlib.h>
#include "game.h"


GameState gameState = START_SCREEN;
Player player;
Collectible items[MAX_ITEMS];
int spawnTimer = 0;


void initGame() {
    player.x = 0.0f;
    player.y = -0.7f;
    player.r = 0.2f; player.g = 0.6f; player.b = 1.0f;
    player.score = 0;
    player.lives = 3;

    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
    }

    spawnTimer = 0;
    gameState = START_SCREEN;
}

void resetGame() {
    player.x = 0.0f;
    player.y = -0.7f;
    player.score = 0;
    player.lives = 3;

    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
    }

    spawnTimer = 0;
}

void spawnItem() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            items[i].x = ((rand() % 17) - 8) / 10.0f;
            items[i].y = 0.95f;
            items[i].speed = 0.002f + (rand() % 5) * 0.002f;
            items[i].type = rand() % 2;       
            items[i].active = 1;
            items[i].points = (items[i].type == 0) ? 10 : 5;
            break;
        }
    }
}

void updateGame() {
    spawnTimer++;
    if (spawnTimer >= SPAWN_RATE) {
        spawnItem();
        spawnTimer = 0;
    }

    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            
            items[i].y -= items[i].speed;

            
            if (items[i].y < -1.0f) {
                items[i].active = 0;
                player.lives--;
                if (player.lives <= 0) {
                    gameState = GAME_OVER;
                }
            }
        }
    }

    checkCollisions();
}

void checkCollisions() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            float dx = player.x - items[i].x;
            float dy = player.y - items[i].y;
            float distSq = dx * dx + dy * dy;

            
            if (distSq < 0.032f) {
                items[i].active = 0;
                player.score += items[i].points;
            }
        }
    }
}
