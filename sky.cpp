#include "sky.h"

Sky::Sky()
{
    X = 0;
    Y = 0;
    Z = 0;
    R = 100;
    S = 0.01;
}
void Sky::InitSky(float skyX, float skyY, float skyZ, float skyR, float angle, float speed, GLuint texture)
{
    X = skyX;
    Y = skyY;
    Z = skyZ;
    R = skyR;
    A = angle;
    S = speed;
    T = texture;
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricTexture(qobj, GL_TRUE);
}
void Sky::ShowSky()
{
    A += S;
    glPushMatrix();
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, T);
    glTranslatef(X, Y, Z);
    glRotatef(90, 1, 0, 0);
    glRotatef(A, 0.0f, 0.0f, 1.0f);
    glColor4f(1, 1, 1, 1);
    gluSphere(qobj, R, 512, 512);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
float Sky::GetR()
{
    return R;
}
float Sky::GetX()
{
    return X;
}
float Sky::GetY()
{
    return Y;
}
float Sky::GetZ()
{
    return Z;
}