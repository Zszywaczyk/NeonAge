#ifndef BULLET_H
#define BULLET_H
#include "cmesh.h"
#include "gameobject.h"
#include <iostream>

using namespace std;


class Bullet: public GameObject
{
    private:

    public:
        Bullet();
        void init();
        void render(GLWidget* glwidget);
        void update();
        void printPosition();

        CMesh* m_mesh;
};

#endif // BULLET_H
