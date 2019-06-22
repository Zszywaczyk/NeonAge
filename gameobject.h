#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QVector3D>
#include <QOpenGLTexture>

using namespace std;

class GLWidget;

class GameObject{

    public:

        GameObject();
        QVector3D position = QVector3D(0.0f,0.0f,0.0f);
        QVector3D rotation = QVector3D(0.0f, 0.0f, 0.0f);
        QVector3D scale = QVector3D(1.0f, 1.0f, 1.0f);

        QVector3D previousPosition = QVector3D(0.0f, 0.0f, 0.0f);

        float m_radius = 1.0f;

        struct Material{
            QVector3D ambient = QVector3D(0.2f, 0.2f, 0.2f);
            QVector3D diffuse = QVector3D(0.2f, 0.2f, 0.2f);
            QVector3D specular = QVector3D(0.2f, 0.2f, 0.2f);
            float shininess = 0.2f;
        };

        Material material;

        string m_name;


        //interface:
        virtual void init() = 0;
        virtual void render(GLWidget* glwidget) = 0;
        virtual void update() = 0;
        virtual void printPosition() = 0;
        virtual void move() = 0;

        QVector3D energy = QVector3D(0.0f, 0.0f, 0.0f);

        bool isAlive = true;

        QOpenGLTexture* m_texture = nullptr;
};

#endif // GAMEOBJECT_H
