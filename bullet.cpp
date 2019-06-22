#include "bullet.h"

Bullet::Bullet(){}

void Bullet::init(){
    m_mesh = CMesh::m_meshes["sphere"];

    scale = QVector3D(0.1f, 0.1f, 0.1f);
    m_radius = 0.1f;
    m_name="bullet";
}
void Bullet::render(GLWidget *glwidget){
    //m_name="bullet";
    //m_mesh.render(glwidget);
    m_mesh->render(glwidget);
}
void Bullet::update(){
    position=position+energy*0.3f;
    energy = energy/1.1f;

    float energySum = energy.length();
    m_radius = energySum*0.3f;
    scale = QVector3D(m_radius,m_radius,m_radius);

    if(energySum < 0.1f){
        isAlive = false;
    }

}
void Bullet::printPosition(){
    cout << "Pozycja x:  "<< energy.x()<<"\tPozycja y:  "<< energy.y()<<"\tPozycja z:  "<< energy.z()<<endl;
}
void Bullet::move(){

}
