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
    void printPosition();
    CMesh* m_mesh;
    void move();
};

#endif // CUBE_H
