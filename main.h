#ifndef _MAIN_H_
#define _MAIN_H_

#define MAX_SIZE 1000
#define PI 3.1415926
#define FIRST_VIEW 0

extern int drawMode;
extern int wWidth;
extern int wHeight;
extern float whRatio;

void redraw();
void reshape(int width, int height);
void key(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void onmousemove(int x, int y);

void init_main();

#endif
