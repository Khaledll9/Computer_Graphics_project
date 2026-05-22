#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "renderer.h"

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.3);
    glVertex2f(-0.3, -0.3);
    glVertex2f(0.3, -0.3);
    glEnd();
}


void drawBall() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float angle = i * 2.0f * 3.1416f / 100;
        glVertex2f(0.1f * cos(angle), 0.1f * sin(angle));
    }
    glEnd();
}

void drawPolygon() {
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9, 0.9f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(-0.1f, 0.1f);
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(0.1f, -0.1f);
    glEnd();
}


void renderText(float x, float y, void* font, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void drawPlayer() {
    glPushMatrix();
    glTranslatef(player.x, player.y, 0.0f);
    glScalef(0.5f, 0.5f, 1.0f);
    glColor3f(player.r, player.g, player.b);
    drawTriangle();
    glPopMatrix();
}


void drawCollectible(Collectible* item) {
    glPushMatrix();
    glTranslatef(item->x, item->y, 0.0f);

    if (item->type == 0) {
        glColor3f(1.0f, 0.84f, 0.0f);   
        drawBall();
    } else {
        glColor3f(0.0f, 0.9f, 0.9f);     
        drawPolygon();
    }

    glPopMatrix();
}


void drawBorder() {
    glColor3f(0.35f, 0.35f, 0.55f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.95f, -0.95f);
    glVertex2f(0.95f, -0.95f);
    glVertex2f(0.95f, 0.95f);
    glVertex2f(-0.95f, 0.95f);
    glEnd();
    glLineWidth(1.0f);
}


void drawHUD() {
    char buffer[32];

    
    sprintf_s(buffer, sizeof(buffer), "Score: %d", player.score);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.18f, 0.92f, GLUT_BITMAP_HELVETICA_18, buffer);

    
    for (int i = 0; i < player.lives; i++) {
        glPushMatrix();
        glTranslatef(-0.88f + i * 0.1f, 0.92f, 0.0f);
        glScalef(0.3f, 0.3f, 1.0f);
        glColor3f(1.0f, 0.2f, 0.2f);
        drawTriangle();
        glPopMatrix();
    }
}


void drawStartScreen() {
    glColor3f(1.0f, 0.85f, 0.0f);
    renderText(-0.45f, 0.65f, GLUT_BITMAP_TIMES_ROMAN_24, "SHAPE COLLECTOR");

    glColor3f(0.6f, 0.8f, 1.0f);
    renderText(-0.50f, 0.35f, GLUT_BITMAP_HELVETICA_18, "Catch the falling shapes!");

    glColor3f(0.7f, 0.9f, 0.7f);
    renderText(-0.65f, 0.05f, GLUT_BITMAP_HELVETICA_18, "Use WASD / Arrow Keys to Move");
    renderText(-0.55f, -0.15f, GLUT_BITMAP_HELVETICA_18, "Mouse Click to Change Color");

    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.35f, -0.45f, GLUT_BITMAP_HELVETICA_18, "Press SPACE to Start");

    
    glPushMatrix();
    glTranslatef(-0.75f, 0.75f, 0.0f);
    glColor3f(1.0f, 0.85f, 0.0f);
    drawBall();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75f, 0.75f, 0.0f);
    glColor3f(0.0f, 0.9f, 0.9f);
    drawPolygon();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f);
    glColor3f(0.2f, 0.6f, 1.0f);
    drawTriangle();
    glPopMatrix();
}


void drawGameOverScreen() {
    char buffer[32];

    glColor3f(1.0f, 0.2f, 0.2f);
    renderText(-0.35f, 0.35f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");

    sprintf_s(buffer, sizeof(buffer), "Final Score: %d", player.score);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.30f, 0.0f, GLUT_BITMAP_HELVETICA_18, buffer);

    glColor3f(0.5f, 0.8f, 0.5f);
    renderText(-0.45f, -0.30f, GLUT_BITMAP_HELVETICA_18, "Press SPACE to Restart");
}



void drawGameScene() {
    switch (gameState) {
        case START_SCREEN:
            drawStartScreen();
            break;

        case PLAYING:
            drawBorder();
            drawHUD();
            drawPlayer();
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (items[i].active) {
                    drawCollectible(&items[i]);
                }
            }
            break;

        case GAME_OVER:
            drawGameOverScreen();
            break;
    }
}
