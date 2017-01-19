#ifndef _MODULE_H_
#define _MODULE_H_

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct
{
    float x;
    float y;
    float z;
} VERTEX;

typedef struct
{
    float left;
    float right;
    float top;
    float bottom;
    float front;
    float back;
} BOX;

class Object
{

public:
    typedef std::vector<VERTEX>::iterator viter;
    typedef std::vector< std::pair<float, float> >::iterator titer;
    typedef std::vector<VERTEX>::iterator niter;
    typedef std::vector<int>::iterator fiter;
    Object(viter vbeg, viter vend, titer tbeg, titer tend, niter nbeg, niter nend, fiter fbeg, fiter fend, int r, int c, std::string mat)
        :vertexs(vbeg, vend), texcoords(tbeg, tend), normals(nbeg, nend), faces(fbeg, fend), row(r), col(c), material(mat)
    { }
    Object() { }

    std::vector<VERTEX> vertexs;
    std::vector< std::pair<float, float> > texcoords;
    std::vector<VERTEX> normals;
    std::vector<int> faces;
    std::string material;
    int row, col;
};

class Material
{

public:
    Material(float *a, float *d, float *s, float *e, unsigned m)
    {
        ambient[0] = *a;
        ambient[1] = *(a + 1);
        ambient[2] = *(a + 2);
        ambient[3] = 0;
        diffuse[0] = *d;
        diffuse[1] = *(d + 1);
        diffuse[2] = *(d + 2);
        diffuse[3] = 0;
        specular[0] = *s;
        specular[1] = *(s + 1);
        specular[2] = *(s + 2);
        specular[3] = 0;
        emission[0] = *e;
        emission[1] = *(e + 1);
        emission[2] = *(e + 2);
        emission[3] = 0;
        map = m;
    }
    Material()
    {
        ambient[0] = 0;
        ambient[1] = 0;
        ambient[2] = 0;
        ambient[3] = 0;
        diffuse[0] = 1.0;
        diffuse[1] = 1.0;
        diffuse[2] = 1.0;
        diffuse[3] = 0;
        specular[0] = 1.0;
        specular[1] = 1.0;
        specular[2] = 1.0;
        specular[3] = 0;
        emission[0] = 0;
        emission[1] = 0;
        emission[2] = 0;
        emission[3] = 0;
        map = 0;
    }
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    unsigned map;
};

inline int power_of_two(int n)
{
    if (n <= 0) return 0;
    return (n&(n - 1)) == 0;
}

inline bool startswith(string &a, string &b)
{
    if (a.size()<b.size())
    {
        return false;
    }
    return a.substr(0, b.size()) == b;
}

void ReadMtl(string &cd, string mtlfile, map<string, Material> &mat);
void ReadObj(string &cd, string file, set<string> &n, map<string, Object> &m, map<string, Material> &matname);
void setMaterial(Material &mat);
void GetBox(set<string> &objname, map<string, Object> &objmap, vector<BOX> &modulebox);
int IsCrash(string name);
int InBox(vector<BOX> modulebox, float x, float y, float z);
void DrawObj(set<string> objname, map<string, Object> objmap, map<string, Material> matname);

#endif

