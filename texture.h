#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glut.h>
#include <stdio.h>
#include <windows.h>
#define BITMAP_ID 0x4D42

unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
void texload(const char *filename, GLuint texture);

#endif

