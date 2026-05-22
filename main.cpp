#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "renderer.h"
#include "input.h"


void init() {
    glClearColor(0.05f, 0.05f, 0.12f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawGameScene();

    glutSwapBuffers();
}

void update(int value) {
    if (gameState == PLAYING) {
        updateGame();
    }
    glutPostRedisplay();
    glutTimerFunc(17, update, 0);  
}

int main(int argc, char** argv) {
    
    srand((unsigned int)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Shape Collector - Computer Graphics Game");

    init();
    initGame();

    
    glutDisplayFunc(display);
    glutTimerFunc(17, update, 0);  
    glutKeyboardFunc(handleFreeKeys);
    glutSpecialFunc(handleSpecialKeys);
    glutMouseFunc(mouseHandler);

    glutMainLoop();
    return 0;
}
}