#include "ground.h"

Ground::Ground()
{
    X = 0;
    Y = 0;
    Z = 0;
    R = 100;
}
void Ground::InitGround(float skyX, float skyY, float skyZ, float skyR, GLuint texture)
{
    X = skyX;
    Y = skyY;
    Z = skyZ;
    R = skyR;
    T = texture;
}
void Ground::ShowGround()
{
    int i;
    GLfloat point[4][3] = {{R, 0, R}, {-R, 0, R}, {-R, 0, -R}, {R, 0, -R}};
    int dir[4][2] = {{1, 1},{1, 0},{0, 0},{0, 1}};
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glCullFace(GL_BACK);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, T);
    glTranslatef(X, Y, Z);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++)
    {
        glTexCoord2iv(dir[i]);
        glVertex3fv(point[i]);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
float Ground::GetR()
{
    return R;
}
float Ground::GetX()
{
    return X;
}
float Ground::GetY()
{
    return Y;
}
float Ground::GetZ()
{
    return Z;
}