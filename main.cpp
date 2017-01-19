#include <stdio.h>
#include <glut.h>
#include "main.h"
#include "menu.h"
#include "play.h"

float whRatio;
int wWidth = 0;
int wHeight = 0;
int drawMode;

void key(unsigned char key, int x, int y)
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {

    }
    else if (drawMode == PLAY)
    {
        play_key(key, x, y);
    }
}

void mouse(int button, int state, int x, int y)
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {
        menu_mouse(button, state, x, y);
    }
    else if (drawMode)
    {
        play_mouse(button, state, x, y);
    }
}

void onmousemove(int x, int y)
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {

    }
    else if (drawMode == PLAY)
    {
        play_onmousemove(x, y);
    }
}

void updateView(int width, int height)
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {
        menu_updateView(width, height);
    }
    else if (drawMode == PLAY)
    {
        play_updateView(width, height);
    }
}

void redraw()
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {
        int play = 0;
        if (drawMode == LOAD)
            play = 1;
        menu_redraw();
        if (play)
        {
            init_play();
            play_updateView(wWidth, wHeight);
            drawMode = PLAY;
        }
    }
    else if (drawMode == PLAY)
    {
        play_redraw();
    }
}

void reshape(int width, int height)
{
    if (drawMode == MENU || drawMode == HELP || drawMode == LOAD)
    {
        menu_reshape(width, height);
    }
    else if (drawMode == PLAY)
    {
        play_reshape(width, height);
    }
}

void idle()
{
    glutPostRedisplay();
}

void init_main()
{
    drawMode = MENU;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(720, 720);
    int windowHandle = glutCreateWindow("NoPigLives");
    init_main();
    init_menu();
    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(onmousemove);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}