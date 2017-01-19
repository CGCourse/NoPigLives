#include "module.h"
#include "texture.h"

void ReadMtl(string &cd, string mtlfile, map<string, Material> &mat)
{
    ifstream in;
    string line, word, ptname, ntname, fname;
    GLuint map;
    bool hasmap = false;
    float ambient[3] = {0.0,0.0,0.0};
    float diffuse[3] = {0.0,0.0,0.0};
    float specular[3] = {0.0,0.0,0.0};
    float emission[3] = {0.0,0.0,0.0};
    cout << "Reading " << mtlfile << endl;
    in.open((cd + "/" + mtlfile).c_str());
    if (!in)
    {
        cout << "Read mtl error !" << endl;
        return;
    }
    while (getline(in, line))
    {
        if (line.size() == 0 || line[0] == '#') continue;
        istringstream is(line);
        is >> word;
        if (word == "newmtl")
        {
            if (!ptname.empty())
            {
                if (hasmap)
                {
                    mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, map)));
                }
                else
                {
                    mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, 0)));
                }
            }
            is >> ptname;
            hasmap = false;
        }
        else if (word == "Ka")
        {
            is >> ambient[0] >> ambient[1] >> ambient[2];
        }
        else if (word == "Kd")
        {
            is >> diffuse[0] >> diffuse[1] >> diffuse[2];
        }
        else if (word == "Ks")
        {
            is >> specular[0] >> specular[1] >> specular[2];
        }
        else if (word == "Ke")
        {
            is >> emission[0] >> emission[1] >> emission[2];
        }
        else if (word == "map_Kd")
        {
            is >> fname;
            glGenTextures(1, &map);
            texload((cd + "/" + fname).c_str(), map);
            hasmap = true;
        }
    }
    if (!ptname.empty())
    {
        if (hasmap)
        {
            mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, map)));
        }
        else
        {
            mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, 0)));
        }
    }
    in.close();
}

void ReadObj(string &cd, string file, set<string> &n, map<string, Object> &m, map<string, Material> &matname)
{
    ifstream in;
    vector<VERTEX> vertexs;
    vector< pair<float, float> > texcoords;
    vector<VERTEX> normals;
    vector<int> faces;
    int row = 0, col = 0;
    string line, word, goname, mtlname;
    char Buffer[MAX_PATH];

    cout << "Reading " << file << endl;
    if (file.find(":") != string::npos)
    {
        cd = string(file.begin(), file.begin() + file.rfind("/"));
    }
    else if (startswith(file, string("./")))
    {
        GetCurrentDirectoryA(MAX_PATH, Buffer);
        cd = Buffer + string(file.begin() + 1, file.begin() + file.rfind("/"));
    }
    else if (startswith(file, string("../")))
    {
        GetCurrentDirectoryA(MAX_PATH, Buffer);
        cd = Buffer;
        cd = string(cd.begin(), cd.begin() + cd.rfind("/"));
        cd = cd + string(file.begin() + 2, file.begin() + file.rfind("/"));
        cout << cd << endl;
    }
    else
    {
        GetCurrentDirectoryA(MAX_PATH, Buffer);
        if (file.rfind("/") != string::npos)
        {
            cd = Buffer + string("/") + string(file.begin(), file.begin() + file.rfind("/"));
        }
        else
        {
            cd = Buffer;
        }
    }

    in.open(file.c_str());
    if (!in)
    {
        cout << "Read obj error !" << endl;
        exit(0);
    }
    while (getline(in, line))
    {
        if (line.size() == 0 || line[0] == '#') continue;
        istringstream is(line);
        is >> word;
        if (word == "v")
        {
            VERTEX p;
            is >> p.x >> p.y >> p.z;
            vertexs.push_back(p);
        }
        else if (word == "vt")
        {
            pair<float, float> p;
            is >> p.first >> p.second;
            texcoords.push_back(p);
        }
        else if (word == "vn")
        {
            VERTEX p;
            is >> p.x >> p.y >> p.z;
            normals.push_back(p);
        }
        else if (word == "o" || word == "g")
        {
            if (!goname.empty() && !faces.empty())
            {
                Object obj(vertexs.begin(), vertexs.end(), texcoords.begin(), texcoords.end(), normals.begin(), normals.end(), faces.begin(), faces.end(), row, col, mtlname);
                while (n.count(goname) != 0)
                {
                    goname.push_back('0');
                }
                m.insert(make_pair(goname, obj));
                n.insert(goname);
                faces.clear();
            }
            is >> goname;
        }
        else if (word == "f")
        {
            int r = 0, c = 0;
            while (is >> word)
            {
                c = count(word.begin(), word.end(), '/');
                if (c == 0)
                {
                    faces.push_back(atoi(word.c_str()));
                }
                else if (c == 1)
                {
                    faces.push_back(atoi(string(word.begin(), word.begin() + word.find("/")).c_str()));
                    faces.push_back(atoi(string(word.begin() + word.find("/") + 1, word.end()).c_str()));
                }
                else if (c == 2)
                {
                    int a = word.find("/");
                    int b = word.find("/", a + 1);
                    faces.push_back(atoi(string(word.begin(), word.begin() + a).c_str()));
                    faces.push_back(atoi(string(word.begin() + a + 1, word.begin() + b).c_str()));
                    faces.push_back(atoi(string(word.begin() + b + 1, word.end()).c_str()));
                }
                ++r;
            }
            row = r;
            col = c + 1;
        }
        else if (word == "mtllib")
        {
            is >> word;
            //ReadMtl(cd, word, matname);
        }
        else if (word == "usemtl")
        {
            is >> mtlname;
        }
    }
    if (!goname.empty() && !faces.empty())
    {
        Object obj(vertexs.begin(), vertexs.end(), texcoords.begin(), texcoords.end(), normals.begin(), normals.end(), faces.begin(), faces.end(), row, col, mtlname);
        while (n.count(goname) != 0)
        {
            goname.push_back('0');
        }
        m.insert(make_pair(goname, obj));
        n.insert(goname);
        faces.clear();
    }
    in.close();
}

void setMaterial(Material &mat)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.emission);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
}

void GetBox(set<string> &objname, map<string, Object> &objmap, vector<BOX> &modulebox)
{
    set<string>::iterator it = objname.begin();
    for (it = objname.begin(); it != objname.end(); ++it)
    {
        Object temp = objmap[*it];
        vector<int>::iterator iter = temp.faces.begin();
        if (!IsCrash(*it))
            continue;
        BOX box;
        float fx, fy, fz;
        box.left = temp.vertexs[*iter - 1].x;
        box.right = temp.vertexs[*iter - 1].x;
        box.top = temp.vertexs[*iter - 1].y;
        box.bottom = temp.vertexs[*iter - 1].y;
        box.front = temp.vertexs[*iter - 1].z;
        box.back = temp.vertexs[*iter - 1].z;
        for (iter = temp.faces.begin(); iter != temp.faces.end(); )
        {
            fx = temp.vertexs[*iter - 1].x;
            fy = temp.vertexs[*iter - 1].y;
            fz = temp.vertexs[*iter - 1].z;
            if (box.left > fx)
                box.left = fx;
            if (box.right < fx)
                box.right = fx;
            if (box.top < fy)
                box.top = fy;
            if (box.bottom > fy)
                box.bottom = fy;
            if (box.front < fz)
                box.front = fz;
            if (box.back > fz)
                box.back = fz;
            if (temp.col == 1)
                iter++;
            else if (temp.col == 2)
                iter += 2;
            else if (temp.col == 3 && !temp.texcoords.empty())
                iter += 3;
            else
                iter += 3;
        }
        modulebox.push_back(box);
    }
}

int IsCrash(string name)
{
    if (name.find("concreteFance") != string::npos
        || name.find("carton") != string::npos
        || name.find("trashCan") != string::npos
        || name.find("null_6") != string::npos
        || name.find("null_7") != string::npos
        || name.find("fance") != string::npos
        || name == "pasted__pasted__null_"
        || name == "pasted__null_"
        || name == "null_")
        return 1;
    return 0;
}

int InBox(vector<BOX> modulebox, float x, float y, float z)
{
    vector<BOX>::iterator it;
    for (it = modulebox.begin(); it != modulebox.end(); it++)
    {
        if (x >= (*it).left && x <= (*it).right
            && y >= (*it).bottom && y <= (*it).top
            && z >= (*it).back && z <= (*it).front)
            return 1;
    }
    return 0;
}

void DrawObj(set<string> objname, map<string, Object> objmap, map<string, Material> matname)
{
    set<string>::iterator it = objname.begin();
    for (it = objname.begin(); it != objname.end(); ++it)
    {
        Object temp = objmap[*it];
        setMaterial(matname[temp.material]);
        glBindTexture(GL_TEXTURE_2D, matname[temp.material].map);
        if (temp.row == 3)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_QUADS);
        vector<int>::iterator iter = temp.faces.begin();
        if (temp.col == 1)
        {
            while (iter != temp.faces.end())
            {
                glVertex3f(temp.vertexs[*iter - 1].x, temp.vertexs[*iter - 1].y, temp.vertexs[*iter - 1].z);
                ++iter;
            }
        }
        else if (temp.col == 2)
        {
            while (iter != temp.faces.end())
            {
                glTexCoord2f(temp.texcoords[*(iter + 1) - 1].first, temp.texcoords[*(iter + 1) - 1].second);
                glVertex3f(temp.vertexs[*iter - 1].x, temp.vertexs[*iter - 1].y, temp.vertexs[*iter - 1].z);
                iter += 2;
            }
        }
        else if (temp.col == 3 && !temp.texcoords.empty())
        {
            while (iter != temp.faces.end())
            {
                glNormal3f(temp.normals[*(iter + 2) - 1].x, temp.normals[*(iter + 2) - 1].y, temp.normals[*(iter + 2) - 1].z);
                glTexCoord2f(temp.texcoords[*(iter + 1) - 1].first, temp.texcoords[*(iter + 1) - 1].second);
                glVertex3f(temp.vertexs[*iter - 1].x, temp.vertexs[*iter - 1].y, temp.vertexs[*iter - 1].z);
                iter += 3;
            }
        }
        else
        {
            while (iter != temp.faces.end())
            {
                glNormal3f(temp.normals[*(iter + 2) - 1].x, temp.normals[*(iter + 2) - 1].y, temp.normals[*(iter + 2) - 1].z);
                glVertex3f(temp.vertexs[*iter - 1].x, temp.vertexs[*iter - 1].y, temp.vertexs[*iter - 1].z);
                iter += 3;
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnd();
    }
}
