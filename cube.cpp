#include "cube.h"
#include <iostream>
using namespace std;

Cube::Cube()
{
    m_name = "cube";
}

void Cube::init()
{
    m_mesh.generateCube(1.0f,1.0f,1.0f);
}

void Cube::render(GLWidget* glwidget)
{
    m_mesh.render(glwidget);
}

void Cube::update()
{
    //wykorzystanie energii
    //energy = energy + energy;
    position = position+energy;
    //wytraanie energii
    energy = energy / 1.2f;
}

void Cube::printPosition(){
    cout << "Pozycja x:  "<< energy.x()<<"\tPozycja y:  "<< energy.y()<<"\tPozycja z:  "<< energy.z()<<endl;
}
