#ifndef _SKY_H_
#define _SKY_H_
#include <glut.h>
class Sky
{
public:
    Sky();
    void InitSky(float skyX, float skyY, float skyZ, float skyR, float angle, float speed, GLuint texture);
    void ShowSky(void);
    float GetX(void);
    float GetY(void);
    float GetZ(void);
    float GetR(void);
private:
    float X;
    float Y;
    float Z;
    float R;
    float A;
    float S;
    GLuint T;
    GLUquadricObj *qobj;
};

#endif