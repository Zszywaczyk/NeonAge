#include "player.h"


Player::Player()
{
    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.01f;
}

float Player::getSpeed(){
    return this->speed;
}

void Player::printPosition(){
    cout << "Pozycja x:  "<< position.x()<<"\tPozycja y:  "<< position.y()<<"\tPozycja z:  "<< position.z()<<endl;
}

void Player::printDirection(){
    cout << "\tDirect x:  "<< direction.x()<<"\tDirect y:  "<< direction.y()<<"\tDirect z:  "<< direction.z()<<endl;
}
