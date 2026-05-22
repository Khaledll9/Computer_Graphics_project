#include <GL/freeglut.h>
#include <stdlib.h>
#include "input.h"
#include "game.h"


void handleFreeKeys(unsigned char key, int mx, int my) {
    switch (key) {
        
        case ' ':
            if (gameState == START_SCREEN) {
                gameState = PLAYING;
            } else if (gameState == GAME_OVER) {
                resetGame();
                gameState = PLAYING;
            }
            break;

        
        case 'a': case 'A':
            if (gameState == PLAYING && player.x > -BOUNDARY)
                player.x -= PLAYER_STEP;
            break;
        case 'd': case 'D':
            if (gameState == PLAYING && player.x < BOUNDARY)
                player.x += PLAYER_STEP;
            break;
        case 'w': case 'W':
            if (gameState == PLAYING && player.y < BOUNDARY)
                player.y += PLAYER_STEP;
            break;
        case 's': case 'S':
            if (gameState == PLAYING && player.y > -BOUNDARY)
                player.y -= PLAYER_STEP;
            break;

        
        case 27:
            exit(0);
            break;
    }

        glutPostRedisplay();
}



void handleSpecialKeys(int key, int mx, int my) {
    switch (key) {
        case GLUT_KEY_LEFT:
            if (gameState == PLAYING && player.x > -BOUNDARY)
                player.x -= PLAYER_STEP;
            break;
        case GLUT_KEY_RIGHT:
            if (gameState == PLAYING && player.x < BOUNDARY)
                player.x += PLAYER_STEP;
            break;
        case GLUT_KEY_UP:
            if (gameState == PLAYING && player.y < BOUNDARY)
                player.y += PLAYER_STEP;
            break;
        case GLUT_KEY_DOWN:
            if (gameState == PLAYING && player.y > -BOUNDARY)
                player.y -= PLAYER_STEP;
            break;
    }

    glutPostRedisplay();
}



void mouseHandler(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (gameState == PLAYING) {
            
            player.r = 0.3f + (rand() % 70) / 100.0f;
            player.g = 0.3f + (rand() % 70) / 100.0f;
            player.b = 0.3f + (rand() % 70) / 100.0f;
        }
    }

    glutPostRedisplay();
}
