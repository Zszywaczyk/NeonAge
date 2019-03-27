#ifndef CUBE_H
#define CUBE_H

#include "cmesh.h"
#include "gameobject.h"



class Cube: public GameObject
{
public:
    Cube();


    void init();
    void render(GLWidget* glwidget);
    void update();
    CMesh m_mesh;
};

#endif // CUBE_H
