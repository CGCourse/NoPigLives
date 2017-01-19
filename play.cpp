#include <stdio.h>
#include <time.h>
#include <glut.h>
#include "main.h"
#include "menu.h"
#include "play.h"
#include "texture.h"
#include "module.h"
#include "sky.h"
#include "ground.h"
#include "road.h"
#include "jump.h"

#define MAX_SIZE 1000
#define PI 3.1415926
#define FIRST_VIEW 0
#define THIRD_VIEW 1
#define MAXTIME 60

set<string> city_name, pig_name, bird_name, star_name;
map<string, Object> city_map, pig_map, bird_map, star_map;
map<string, Material> city_mat, pig_mat, bird_mat, star_mat;
vector<BOX> city_box;
string cd, pigcd, birdcd, starcd;
GLuint sky_tex, ground_tex;
Sky sky_box;
Ground ground;
JudgeRange jr;
float pig_size = 2;
jumpTrigger jt(pig_size);

float White[] = {1.0,1.0,1.0,1.0};

float moldx = 0;
float moldy = 0;
float day_speed = 0.1;

float Alpha = 0;
float Beta = 0;
float radius = 10;
float center[] = {0, 0, 0};
float eye[] = {-100, 2, 70};
float pig_pos[] = {-100, 2, 70};
float sun_pos[3];
float sun_dis = MAX_SIZE;
float sun_r = 180;
float sensitivity = 0.3;

float birdAlpha;
float starangle;

int firstload = 1;
int viewtype = THIRD_VIEW;
int birdfly;
float birdx, birdy, birdz;
float birdspeed;
int birdcometime;
int pigdizzy, gameend;
clock_t start_time, now_time;
int gametime, score;
int checkpoint;

void play_key(unsigned char key, int x, int y)
{
    float move = 0.2;
    float offset_x, offset_z;
    float oldx, oldz;
    oldx = pig_pos[0];
    oldz = pig_pos[2];
    offset_x = move * sin(Alpha);
    offset_z = move * cos(Alpha);
    if ((gameend || pigdizzy) && key != 27 && key != 'q' && key != 'r')
        return;
    if (key == 27 || key == 'q')
    {
        drawMode = MENU;
        menu_updateView(wWidth, wHeight);
    }
    else if (key == 'a')
    {
        pig_pos[0] -= offset_z;
        pig_pos[2] += offset_x;
    }
    else if (key == 'd')
    {
        pig_pos[0] += offset_z;
        pig_pos[2] -= offset_x;
    }
    else if (key == 'r')
    {
        gameend = 0;
        init_play();
        jt.setTrigger(pig_size);
    }
    else if (key == ' ')
    {
        jt.key(key, x, y);
    }
    else if (key == 'v')
    {
        viewtype = !viewtype;
    }
    if (!check(pig_pos[0], pig_pos[1], pig_pos[2]))
    {
        pig_pos[0] = oldx;
        pig_pos[2] = oldz;
    }
}

void play_mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        moldx = x;
        moldy = y;
    }
}

void play_onmousemove(int x, int y)
{
    Alpha -= PI / 180 * (x - moldx) * sensitivity;
    //Beta -= PI / 180 * (y - moldy) * sensitivity;
    if (Beta >= PI / 2 - 0.1)
        Beta = PI / 2 - 0.1;
    if (Beta <= -PI / 2 + 0.1)
        Beta = -PI / 2 + 0.1;
    if (viewtype == FIRST_VIEW)
    {
        eye[0] = pig_pos[0] - pig_size * cos(Beta) * sin(Alpha);
        eye[1] = pig_pos[1] + pig_size * sin(Beta);
        eye[2] = pig_pos[2] - pig_size * cos(Beta) * cos(Alpha);
        center[0] = eye[0] - radius * cos(Beta) * sin(Alpha);
        center[1] = eye[1] + radius * sin(Beta);
        center[2] = eye[2] - radius * cos(Beta) * cos(Alpha);
    }
    else
    {
        center[0] = pig_pos[0];
        center[1] = pig_pos[1] + pig_size;
        center[2] = pig_pos[2];
        eye[0] = center[0] + radius * cos(Beta) * sin(Alpha);
        eye[1] = center[1] - radius * sin(Beta);
        eye[2] = center[2] + radius * cos(Beta) * cos(Alpha);
    }
    moldx = x;
    moldy = y;
}

void play_updateView(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    whRatio = (GLfloat)width / (GLfloat)height;
    gluPerspective(45.0f, whRatio, 0.1f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void play_reshape(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }
    wWidth = width;
    wHeight = height;
    play_updateView(wWidth, wHeight);
}

void play_redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, whRatio, 0.1f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (!gameend)
        running();
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    sunlighting();
    draw();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    jt.drawBar();
    showData();

    glutSwapBuffers();
}

void init_play()
{
    if (firstload)
    {
        init_sky();
        init_ground();
        init_module();
        firstload = 0;
    }
    init_view();
    init_data();
}

void init_sky()
{
    glGenTextures(1, &sky_tex);
    texload("module/sky.bmp", sky_tex);
    sky_box.InitSky(0, 0, 0, MAX_SIZE, 0, day_speed, sky_tex);
}

void init_ground()
{
    glGenTextures(1, &ground_tex);
    texload("module/ground.bmp", ground_tex);
    ground.InitGround(0, 0, 0, MAX_SIZE, ground_tex);
}

void init_module()
{
    ReadObj(cd, "module/barrier.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box5.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/box6.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/bucket.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/carchange1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/carchange2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car5.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/car6.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/fence1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/fence2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/fence3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/fence4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/gasStation.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house5.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house6.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house7.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house8.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house9.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house10.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house11.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house12.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house13.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/house14.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/hyrant.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/mailbox.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/phone.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/trafficLight.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/trafficLight1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road5.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road6.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road7.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/road8.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree3.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree4.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree5.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree6.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree7.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree8.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree9.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree10.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree11.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree12.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree13.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree14.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree15.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree16.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree17.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree18.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree19.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree20.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/tree21.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/warning1.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/warning2.obj", city_name, city_map, city_mat);
    ReadObj(cd, "module/warning3.obj", city_name, city_map, city_mat);
    ReadMtl(cd, "all.mtl", city_mat);
    ReadObj(pigcd, "module/pig.obj", pig_name, pig_map, pig_mat);
    ReadMtl(pigcd, "pig.mtl", pig_mat);
    ReadObj(birdcd, "module/bird.obj", bird_name, bird_map, bird_mat);
    ReadMtl(birdcd, "bird.mtl", bird_mat);
    ReadObj(starcd, "module/star.obj", star_name, star_map, star_mat);
    ReadMtl(starcd, "star.mtl", star_mat);
    GetBox(city_name, city_map, city_box);
}

void init_view()
{
    Alpha = 0;
    Beta = 0;
    pig_pos[0] = 105;
    pig_pos[1] = pig_size;
    pig_pos[2] = 50;
    eye[0] = pig_pos[0] - pig_size * cos(Beta) * sin(Alpha);
    eye[1] = pig_pos[1] + pig_size * sin(Beta);
    eye[2] = pig_pos[2] - pig_size * cos(Beta) * cos(Alpha);
    center[0] = eye[0] - radius * cos(Beta) * sin(Alpha);
    center[1] = eye[1] + radius * sin(Beta);
    center[2] = eye[2] - radius * cos(Beta) * cos(Alpha);
}

void init_data()
{
    start_time = clock();
    now_time = clock();
    gametime = 0;
    srand((unsigned)time(NULL));
    birdcometime = 2 + rand() % 3;
    score = 0;
    checkpoint = 6;
    pigdizzy = 0;
    gameend = 0;
    birdspeed = 0.5;
}

void sunlighting()
{
    sun_r += day_speed;
    sun_pos[0] = -sun_dis * cos(52 * PI / 180) * sin(sun_r * PI / 180);
    sun_pos[1] = sun_dis * sin(52 * PI / 180);
    sun_pos[2] = sun_dis * cos(52 * PI / 180) * cos(sun_r * PI / 180);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, White);
    glEnable(GL_LIGHT0);
}

void draw()
{
    sky_box.ShowSky();
    ground.ShowGround();
    draw_pig();
    draw_bird();
    draw_star();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPushMatrix();
    DrawObj(city_name, city_map, city_mat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_pig()
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPushMatrix();
    glTranslatef(0, -0.8, 0);
    glTranslatef(pig_pos[0], pig_pos[1], pig_pos[2]);
    glRotatef(Alpha / PI * 180, 0, 1, 0);
    glScalef(1.5, 1.5, 1.5);
    DrawObj(pig_name, pig_map, pig_mat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_bird()
{
    if (birdfly)
    {
        float move = birdspeed;
        birdx += move * sin(birdAlpha);
        birdz += move * cos(birdAlpha);
        birdy -= birdspeed / 8;
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glPushMatrix();
        glTranslatef(birdx, birdy, birdz);
        glRotatef(birdAlpha / PI * 180 - 90, 0, 1, 0);
        DrawObj(bird_name, bird_map, bird_mat);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (birdx >= pig_pos[0] - pig_size && birdx <= pig_pos[0] + pig_size
            && birdy >= pig_pos[1] - pig_size && birdy <= pig_pos[1] + pig_size
            && birdz >= pig_pos[2] - pig_size && birdz <= pig_pos[2] + pig_size)
        {
            pigdizzy = 1;
            starangle = 0;
        }
        if (birdy < 0)
            birdfly = 0;
    }
}

void draw_star()
{
    if (pigdizzy)
    {
        starangle += 3;
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glPushMatrix();
        glTranslatef(pig_pos[0], pig_pos[1] + 2, pig_pos[2]);
        glRotatef(starangle, 0, 1, 0);
        glTranslatef(1, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        DrawObj(star_name, star_map, star_mat);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(pig_pos[0], pig_pos[1] + 2, pig_pos[2]);
        glRotatef(starangle + 120, 0, 1, 0);
        glTranslatef(1, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        DrawObj(star_name, star_map, star_mat);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(pig_pos[0], pig_pos[1] + 2, pig_pos[2]);
        glRotatef(starangle + 240, 0, 1, 0);
        glTranslatef(1, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        DrawObj(star_name, star_map, star_mat);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (starangle > 360 * 3)
            pigdizzy = 0;
    }
}

void showData()
{
    char timebuffer[64];
    char scorebuffer[64];
    char endbuffer[64];
    char helpbuffer[64];
    char *c;
    int cp;
    if (gametime >= MAXTIME)
        gameend = 1;
    else
    {
        now_time = clock();
        gametime = (now_time - start_time) / CLOCKS_PER_SEC;
        if (gametime >= birdcometime)
        {
            birdcome();
            birdcometime += 4 + rand() % 3;
        }
    }
    sprintf(timebuffer, "Time Left: %ds", MAXTIME - gametime);
    if(jr.InScoreArea(pig_pos[0], pig_pos[2]) == checkpoint)
    {
        score++;
        checkpoint = (checkpoint + 11) % 12;
    }
    sprintf(scorebuffer, "Socre: %ds", score);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);    // 选择投影矩阵
    glPushMatrix();                 // 保存原矩阵
    glLoadIdentity();               // 装入单位矩阵
    glOrtho(0, wWidth, 0, wHeight, -1, 1); // 位置正投影
    glMatrixMode(GL_MODELVIEW);     // 选择Modelview矩阵
    glPushMatrix();                 // 保存原矩阵
    glLoadIdentity();               // 装入单位矩阵
    glColor3f(1.0f, 0.0f, 0.0f);
    if (gameend)
    {
        if (score > 1)
            sprintf(endbuffer, "Game End!!! You Scored %d Points", score);
        else
            sprintf(endbuffer, "Game End!!! You Scored %d Point", score);
        glRasterPos2f((wWidth - 300) / 2, (wHeight - 20) / 2);
        for (c = endbuffer; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        sprintf(helpbuffer, "Press R to restart Or Q/Esc to quit");
        glRasterPos2f((wWidth - 300) / 2, (wHeight - 80) / 2);
        for (c = helpbuffer; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    }
    glRasterPos2f(10, 10);
    for (c = timebuffer; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glRasterPos2f(10, 40);
    for (c = scorebuffer; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glMatrixMode(GL_PROJECTION);    // 选择投影矩阵
    glPopMatrix();                  // 重置为原保存矩阵
    glMatrixMode(GL_MODELVIEW);     // 选择Modelview矩阵
    glPopMatrix();                  // 重置为原保存矩阵
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void dispview()
{
    if (viewtype == FIRST_VIEW)
    {
        eye[0] = pig_pos[0] - pig_size * cos(Beta) * sin(Alpha);
        eye[1] = pig_pos[1] + pig_size * sin(Beta);
        eye[2] = pig_pos[2] - pig_size * cos(Beta) * cos(Alpha);
        center[0] = eye[0] - radius * cos(Beta) * sin(Alpha);
        center[1] = eye[1] + radius * sin(Beta);
        center[2] = eye[2] - radius * cos(Beta) * cos(Alpha);
    }
    else
    {
        center[0] = pig_pos[0];
        center[1] = pig_pos[1] + pig_size;
        center[2] = pig_pos[2];
        eye[0] = center[0] + radius * cos(Beta) * sin(Alpha);
        eye[1] = center[1] - radius * sin(Beta);
        eye[2] = center[2] + radius * cos(Beta) * cos(Alpha);
    }
}

void running()
{
    float move = 0.2;
    float offset_x, offset_z;
    if (pigdizzy)
    {
        offset_x = 0;
        offset_z = 0;
    }
    else
    {
        offset_x = move * sin(Alpha);
        offset_z = move * cos(Alpha);
    }
    if (check(pig_pos[0] - offset_x, pig_pos[1], pig_pos[2] - offset_z))
    {
        pig_pos[0] -= offset_x;
        pig_pos[2] -= offset_z;
    }
    if (jt.getTrigger() || jt.getFall())
    {
        float newy = jt.TimerFunction();
        if (check(pig_pos[0], newy, pig_pos[2]))
        {
            pig_pos[1] = newy;
        }
        else
        {
            jt.land(pig_pos[1]);
        }
    }
    else
    {
        if (check(pig_pos[0], pig_pos[1] - 0.1, pig_pos[2]) && pig_pos[1] > pig_size)
            jt.fall();
    }
    dispview();
}

void birdcome()
{
    birdfly = 1;
    birdspeed += 0.05;
    birdAlpha = Alpha;
    birdx = pig_pos[0] - 100 * sin(Alpha);
    birdy = pig_pos[1] + 10;
    birdz = pig_pos[2] - 100 * cos(Alpha);
}

int check(float x, float y, float z)
{
    if (InBox(city_box, x + pig_size, y + pig_size, z + pig_size)
        || InBox(city_box, x + pig_size, y + pig_size, z - pig_size)
        || InBox(city_box, x + pig_size, y - pig_size, z + pig_size)
        || InBox(city_box, x + pig_size, y - pig_size, z - pig_size)
        || InBox(city_box, x - pig_size, y + pig_size, z + pig_size)
        || InBox(city_box, x - pig_size, y + pig_size, z - pig_size)
        || InBox(city_box, x - pig_size, y - pig_size, z + pig_size)
        || InBox(city_box, x - pig_size, y - pig_size, z - pig_size)
        || InBox(city_box, x + pig_size, y, z)
        || InBox(city_box, x - pig_size, y, z)
        || InBox(city_box, x, y + pig_size, z)
        || InBox(city_box, x, y - pig_size, z)
        || InBox(city_box, x, y, z + pig_size)
        || InBox(city_box, x, y, z - pig_size))
        return 0;
    if (!jr.JudgeInRoad(x, z))
        return 0;
    return 1;
}
