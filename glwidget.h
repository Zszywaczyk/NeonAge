#ifndef GLWIDGET_H
#define GLWIDGET_H

//30maja oddanie
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include "cmesh.h"
#include <QElapsedTimer>
#include "player.h"
#include "bullet.h"
#include <vector>

using namespace std;

#define FPP             1
#define TPP             2
#define ISOMETRIC       3
#define FPP_GOD         4


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    public:
        GLWidget(QWidget *parent = nullptr);
        ~GLWidget() override;

        QSize sizeHint() const override;

        friend CMesh;
        Player m_player;
        void keyboardAction();

        void gridOfCubes();
public slots:
        void cleanup();

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int width, int height) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;

        void setTransforms(void);

        void updateGL();

    private:

        struct LightLocStruct
        {
            int position;
            int ambient;
            int diffuse;
        };

        QPoint m_lastPos;
        QOpenGLShaderProgram *m_program;
        int m_projMatrixLoc;
        int m_viewMatrixLoc;
        int m_modelMatrixLoc;
        int m_modelColorLoc;
        LightLocStruct m_lightLoc;

        QMatrix4x4 m_proj;
        QMatrix4x4 m_camera;
        QMatrix4x4 m_world;

        //QMap<QString, CMesh*> m_meshes;

        bool m_keyState[256];

        float m_camDistance = 1.5f;

        QElapsedTimer timer;

        float lastUpdateTime;
        float FPS;

        int cameraType=FPP;
        void cameraTypeUpdateGL();

        vector<GameObject*> m_gameObjects;
        void addObject(GameObject* obj);
};

#endif

