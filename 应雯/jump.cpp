#include <stdio.h>
#include <time.h>
#include <math.h>
#include <glut.h>
#include "jump.h"
#include "play.h"

float v = 40.0;
float g = 50.0;

void jumpTrigger::drawBar() //画力度条
{
    if (isReady)
    {
        barValue += 0.01;
        if (barValue >= 1)
            barValue = 0;
    }
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLineWidth(4.0);
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex2f(0.95, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, -0.5);
    glVertex2f(0.95, -0.5);
    glEnd();
    glColor3f(1.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f(0.95, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, -1.0 + barValue * 0.5);
    glVertex2f(0.95, -1.0 + barValue * 0.5);
    glEnd();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

float jumpTrigger::TimerFunction()
{
    float x, y;
    float jumpPosition;
    if (isTrigger)
    {
        if (isStart)
        {
            startTime = clock();
            isStart = false;
            jumpPosition = ynow;
        }
        else
        {
            //得到当前时间
            float ctime = clock();
            //已经跳跃的时间
            float jumptime = (ctime - startTime) / CLOCKS_PER_SEC;
            //重设y坐标
            jumpPosition = ynow + g * t * jumptime - g / 2 * jumptime * jumptime;
            if (jumpPosition <= ysite)
            {
                ynow = ysite;
                jumpPosition = ysite;
                barValue = 0;
                isStart = true;
                isTrigger = false;
                isFall = false;
            }
        }
    }
    else if (isFall)
    {
        if (isStart)
        {
            startTime = clock();
            isStart = false;
            jumpPosition = ynow;
        }
        else
        {
            //得到当前时间
            float ctime = clock();
            //已经跳跃的时间
            float jumptime = (ctime - startTime) / CLOCKS_PER_SEC;
            //重设y坐标
            jumpPosition = ynow - g / 2 * jumptime * jumptime;
            if (jumpPosition <= ysite)
            {
                ynow = ysite;
                jumpPosition = ynow;
                barValue = 0;
                isStart = true;
                isTrigger = false;
                isFall = false;
            }
        }
    }
    else
        jumpPosition = ynow;
    return jumpPosition;
}

void jumpTrigger::land(float y)
{
    ynow = y;
    barValue = 0;
    isStart = true;
    isTrigger = false;
    isFall = false;
}

void jumpTrigger::fall()
{
    isStart = true;
    isFall = true;
}

void jumpTrigger::key(unsigned char k, int x, int y)
{
    switch (k)
    {
        case ' ':
        {
            if (isTrigger || isFall)
                return;
            if (isReady)
            {
                t = sqrt(barValue * 30.0 / g);
                isTrigger = true;
                isFall = false;
                isStart = true;
                isReady = false;
            }
            else
            {
                isReady = true;
                isStart = true;
                barValue = 0;
            }
        }
        default:
            break;
    }
}
