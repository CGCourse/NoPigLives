#ifndef _PLAY_H_
#define _PLAY_H_

extern float pig_size;

void play_key(unsigned char key, int x, int y);
void play_mouse(int button, int state, int x, int y);
void play_onmousemove(int x, int y);
void play_updateView(int width, int height);
void play_reshape(int width, int height);
void play_redraw();
void init_play();
void init_sky();
void init_ground();
void init_module();
void init_view();
void init_data();
void sunlighting();
void draw();
void draw_pig();
void draw_bird();
void draw_star();
void showData();
void dispview();
void running();
void birdcome();
int check(float x, float y, float z);

#endif
