#ifndef PLAYER_H
#define PLAYER_H
#include <QVector3D>
#include <iostream>

using namespace std;

class Player
{
    private:
        float speed;

    public:
        QVector3D position;
        QVector3D direction;

        Player();
        float getSpeed();
        void printPosition();
        void printDirection();
};

#endif // PLAYER_H
