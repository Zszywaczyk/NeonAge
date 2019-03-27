#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QVector3D>

using namespace std;

class GLWidget;

class GameObject{

        public:
    GameObject();
    QVector3D position = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D rotation = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D scale = QVector3D(1.0f, 1.0f, 1.0f);

    float m_radius = 1.0f;

    QVector3D material_color = QVector3D(1.0f, 1.0f, 1.0f);

    string m_name;

    //interface:
    virtual void init() = 0;
    virtual void render(GLWidget* glwidget) = 0;
    virtual void update() = 0;
};

#endif // GAMEOBJECT_H
