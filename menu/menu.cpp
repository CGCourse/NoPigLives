#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include "main.h"
#include "menu.h"
#include "texture.h"

static GLfloat spin = 0.0;
bool picking = true;
bool pick[6] = {false,false,false,false,false,false};
GLuint selectBuf[SELBUFSIZE];
GLuint icon[3];
GLuint img[3];

void textureCube(GLfloat size)
{
    int i, j;
    const GLfloat x1 = -size / 2, x2 = size / 2;
    const GLfloat y1 = -size / 2, y2 = size / 2;
    const GLfloat z1 = -size / 2, z2 = size / 2;

    GLfloat point[6][4][3] =
    {
        {{x1, y1, z1},{x2, y1, z1},{x2, y2, z1},{x1, y2, z1}},
        {{x1, y1, z1},{x2, y1, z1},{x2, y1, z2},{x1, y1, z2}},
        {{x2, y1, z1},{x2, y2, z1},{x2, y2, z2},{x2, y1, z2}},
        {{x1, y1, z1},{x1, y2, z1},{x1, y2, z2},{x1, y1, z2}},
        {{x1, y2, z1},{x2, y2, z1},{x2, y2, z2},{x1, y2, z2}},
        {{x1, y1, z2},{x2, y1, z2},{x2, y2, z2},{x1, y2, z2}}
    };
    int dir[4][2] = {{1, 1},{1, 0},{0, 0},{0, 1}};

    glBegin(GL_QUADS);
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 4; j++)
        {
            glTexCoord2iv(dir[j]);
            glVertex3fv(point[i][j]);
        }
    }
    glEnd();

}

void spinDisplay()
{
    spin = spin + 0.1;
    if (spin > 360.0)
    {
        spin = spin - 360.0;
    }
}

void show_img(GLuint tex)
{
    GLfloat point[4][3] =
    {
        {8, 8, -5},
        {8, -8, -5},
        {-8, -8, -5},
        {-8, 8, -5}
    };
    int dir[4][2] = {{1, 1},{1, 0},{0, 0},{0, 1}};
    int i;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++)
    {
        glTexCoord2iv(dir[i]);
        glVertex3fv(point[i]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Draw_Menu()
{
    show_img(img[0]);

    spinDisplay();

    glInitNames();
    glPushName(0);

    glLoadName(1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, icon[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    if (picking && pick[1])
    {
        drawMode = HELP;
        pick[1] = false;
    }
    glTranslatef(0, -1, 0);
    glRotatef(spin, 0, 1, 0);
    glRotatef(45, 1, 0, 1);
    textureCube(1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glLoadName(2);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, icon[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    if (picking && pick[2])
    {
        drawMode = LOAD;
        pick[2] = false;
    }
    glTranslatef(-2, -1, 0);
    glRotatef(spin, 0, 1, 0);
    glRotatef(45, 1, 0, 1);
    textureCube(1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glLoadName(3);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, icon[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    if (picking && pick[3])
    {
        exit(0);
    }
    glTranslatef(2, -1, 0);
    glRotatef(spin, 0, 1, 0);
    glRotatef(45, 1, 0, 1);
    textureCube(1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void Draw_Help()
{
    show_img(img[1]);

    spinDisplay();
    glInitNames();
    glPushName(0);
    glLoadName(4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, icon[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    if (picking && pick[4])
    {
        drawMode = MENU;
        pick[4] = false;
    }
    glTranslatef(3, -3, 0);
    glRotatef(spin, 0, 1, 0);
    glRotatef(45, 1, 0, 1);
    textureCube(1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Draw_Load()
{
    show_img(img[2]);
}

void startPicking(int cursorX, int cursorY)
{
    GLint viewport[4];
    glSelectBuffer(SELBUFSIZE, selectBuf);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(cursorX, viewport[3] - cursorY, 5, 5, viewport);
    gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glInitNames();
}

void stopPicking()
{
    int hits;

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    hits = glRenderMode(GL_RENDER);

    if (hits != 0)
        processHits(hits, selectBuf);
}

void processHits(GLint hits, GLuint buffer[])
{
    int i;
    GLuint names, *ptr, minZ, *ptrNames = NULL, numberOfNames;

    ptr = buffer;
    minZ = 0xffffffff;
    for (i = 0; i < hits; i++)
    {
        names = *ptr;
        ptr++;
        if (*ptr < minZ)
        {
            numberOfNames = names;
            minZ = *ptr;
            ptrNames = ptr + 2;
        }
        ptr += names + 2;
    }
    ptr = ptrNames;
    int s = ptrNames[0];
    pick[s] = !pick[s];
}

void pickobject(int x, int y)
{
    startPicking(x, y);
    menu_redraw();
    stopPicking();
}

void init_menu()
{
    glGenTextures(3, icon);
    glGenTextures(3, img);
    texload("menu/help.bmp", icon[0]);
    texload("menu/play.bmp", icon[1]);
    texload("menu/exit.bmp", icon[2]);
    texload("menu/menu.bmp", img[0]);
    texload("menu/tips.bmp", img[1]);
    texload("menu/load.bmp", img[2]);
}

void menu_updateView(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    whRatio = (GLfloat)width / (GLfloat)height;
    gluPerspective(45.0f, whRatio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void menu_reshape(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    wWidth = width;
    wHeight = height;

    menu_updateView(wWidth, wHeight);
}

void menu_redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_pos[] = {0, 0, 10, 0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glEnable(GL_LIGHT0);

    switch (drawMode)
    {
        case MENU: Draw_Menu(); break;
        case HELP: Draw_Help(); break;
        case LOAD: Draw_Load(); break;
        default: break;
    }

    glutSwapBuffers();
}

void menu_mouse(int button, int state, int x, int y)
{
    if (picking && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        pickobject(x, y);
    }
}
