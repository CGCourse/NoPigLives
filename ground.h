#ifndef  _GROUND_H_
#define  _GROUND_H_
#include <glut.h>
class Ground
{
public:
    Ground();
    void InitGround(float skyX, float skyY, float skyZ, float skyR, GLuint texture);
    void ShowGround(void);
    float GetX(void);
    float GetY(void);
    float GetZ(void);
    float GetR(void);
private:
    float X;
    float Y;
    float Z;
    float R;
    GLuint T;
};
#endif
