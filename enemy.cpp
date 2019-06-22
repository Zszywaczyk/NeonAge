#include "enemy.h"

Enemy::Enemy()
{
    energy = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.02f;
}

float Enemy::getSpeed(){
    return this->speed;
}

void Enemy::printPosition(){
    cout << "Pozycja x:  "<< energy.x()<<"\tPozycja y:  "<< energy.y()<<"\tPozycja z:  "<< energy.z()<<endl;
}

void Enemy::printDirection(){
    cout << "\tDirect x:  "<< direction.x()<<"\tDirect y:  "<< direction.y()<<"\tDirect z:  "<< direction.z()<<endl;
}

void Enemy::init(){
    m_mesh = CMesh::m_meshes["enemy"];

    scale = QVector3D(0.1f, 0.1f, 0.1f);
    m_radius = 0.3f;
    m_name = "Enemy";
}

void Enemy::render(GLWidget* glwidget)
{
    m_mesh->render(glwidget);
}

void Enemy::update()
{
    rotation.setY(90-atan2(direction.z(), direction.x())*180/3.14f);
    position= position+energy;
    //wykorzystanie energii

    //wytracanie energii
    energy = energy/1.2f;
}

void Enemy::move(){
    //energy.setX(energy.x() + direction.x() * speed);
    //energy.setZ(energy.z() + direction.z() * speed);
}
