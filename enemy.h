#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"
#include "cmesh.h"
#include <iostream>
#include <math.h>
using namespace std;


class Enemy : public GameObject {
private:

public:
    Enemy();

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

#endif // ENEMY_H
