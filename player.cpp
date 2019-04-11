#include "player.h"


Player::Player()
{
    energy = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.01f;
}

float Player::getSpeed(){
    return this->speed;
}

void Player::printPosition(){
    cout << "Pozycja x:  "<< energy.x()<<"\tPozycja y:  "<< energy.y()<<"\tPozycja z:  "<< energy.z()<<endl;
}

void Player::printDirection(){
    cout << "\tDirect x:  "<< direction.x()<<"\tDirect y:  "<< direction.y()<<"\tDirect z:  "<< direction.z()<<endl;
}

void Player::init(){
    m_mesh.generateMeshFromObjFile("resources/bunny.obj");
    scale = QVector3D(0.1f, 0.1f, 0.1f);
    m_radius = 0.1f;

    m_name="Player";
}

void Player::render(GLWidget* glwidget)
{
    m_mesh.render(glwidget);
}

void Player::update()
{
    rotation.setY(90-atan2(direction.z(), direction.x())*180/3.14f);
    position= position+energy;
    //wykorzystanie energii

    //wytracanie energii
    energy = energy/1.2f;

}
