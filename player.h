#ifndef PLAYER_H
#define PLAYER_H
#include "gameobject.h"
#include <QVector3D>
#include <iostream>
#include "cmesh.h"
#include <math.h>

using namespace std;

class Player: public GameObject
{
    private:


    public:
        Player();

        QVector3D direction;

        float getSpeed();
        void printPosition();
        void printDirection();
        float speed;

        CMesh* m_mesh;

        void init();
        void render(GLWidget* glwidget);
        void update();
        float hp =100;
        const float MAX_HP = 100;
        void move();
};

#endif // PLAYER_H
