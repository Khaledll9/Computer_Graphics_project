#ifndef RENDERER_H
#define RENDERER_H
#include "game.h"


void drawStartScreen();
void drawGameScene();
void drawGameOverScreen();
void drawHUD();
void drawBorder();
void drawPlayer();
void drawCollectible(Collectible* item);
void renderText(float x, float y, void* font, const char* text);

void drawTriangle();
void drawBall();
void drawPolygon();

#endif
