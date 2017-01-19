#ifndef _MENU_H_
#define _MENU_H_

#define MENU 0
#define HELP 1
#define LOAD 2
#define PLAY 3
#define SELBUFSIZE 512

void textureCube(GLfloat size);
void spinDisplay();
void Draw_Menu();
void Draw_Help();
void Draw_Load();
void startPicking(int cursorX, int cursorY);
void stopPicking();
void processHits(GLint hits, GLuint buffer[]);
void pickobject(int x, int y);
void init_menu();
void menu_updateView(int width, int height);
void menu_reshape(int width, int height);
void menu_redraw();
void menu_mouse(int button, int state, int x, int y);

#endif
