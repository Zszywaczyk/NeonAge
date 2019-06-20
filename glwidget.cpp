#include "cube.h"
#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <iostream>
#include <qstack.h>

using namespace std;


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true); // I/O śledzenie myszy

    //niewidzialność myszy:
    QCursor c = cursor();
    c.setShape(Qt::CursorShape::BlankCursor);
    setCursor(c);
    //---

}

GLWidget::~GLWidget()
{
    /*for(auto it = m_meshes.begin() ; it != m_meshes.end(); it++)
        delete it.value();*/

    cleanup();
}

QSize GLWidget::sizeHint() const
{
    return QSize(1000, 800);
}

void GLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();

    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GLWidget::gridOfCubes(){
    for(int i = 0 ; i < 5 ; i++){
        for(int j = 0 ; j < 7 ; j++){
            // Wskaźnik do Cube'a z tworzeniem obiektu.
            // MUSI BYĆ WSKAŹNIK !!!
            Cube* cube = new Cube();
            // Ustawienie pozycji Cube'a
            cube->position.setX(j * 1 - 3);
            cube->position.setY(0);
            cube->position.setZ(i * 1 - 6);
            // Kolor Cube'a. Zwykły gradient.
            cube->material_color.setX(i * 0.2f);
            cube->material_color.setY(0.5f);
            cube->material_color.setZ(j * 0.1f);
            // Wielkość Cube'a.
            // Ustawienie w jednej linijce
            // zamiast osobno dla X, Y i Z.
            cube->scale = QVector3D(0.3f, 0.3f, 0.3f);
            cube->m_radius = 0.3 * sqrt(3 * cube->scale.x() * cube->scale.x());
            // Dodanie obiektu do sceny.
            addObject(cube);
        }
    }
}

void GLWidget::initializeGL()
{
    //addObject(&m_player);
    //pole kwadratow
    //gridOfCubes();


    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.2f, 0.3f, 1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    //CMesh::loadAllMeshes();

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "resources/shader.vs");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "resources/shader.fs");
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program->uniformLocation("viewMatrix");
    m_modelMatrixLoc = m_program->uniformLocation("modelMatrix");
    m_modelColorLoc = m_program->uniformLocation("modelColor");
    m_lightLoc.position = m_program->uniformLocation("light.position");
    m_lightLoc.ambient = m_program->uniformLocation("light.ambient");
    m_lightLoc.diffuse = m_program->uniformLocation("light.diffuse");

    /*m_meshes.insert("Cube", new CMesh);
    m_meshes["Cube"]->generateCube(1.0f, 1.0f, 1.0f);

    m_meshes.insert("Sphere", new CMesh);
    m_meshes["Sphere"]->generateSphere(0.5f, 24);

    m_meshes.insert("Bunny", new CMesh);
    m_meshes["Bunny"]->generateMeshFromObjFile("resources/bunny.obj");*/

    m_program->release();
	
	lastUpdateTime = 0;
	timer.start();
	FPS = 60;

    CMesh::loadAllMeshes();
    addObject(&m_player);
    gridOfCubes();

    //m_robotPosition = QVector3D(0.0f, 0.0f, 0.0f);
	
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QStack<QMatrix4x4> worldMatrixStack;

    m_program->bind();

    m_program->setUniformValue(m_lightLoc.position, QVector3D(0.0f, 0.0f, 15.0f));
    m_program->setUniformValue(m_lightLoc.ambient, QVector3D(0.1f, 0.1f, 0.1f));
    m_program->setUniformValue(m_lightLoc.diffuse, QVector3D(0.9f, 0.9f, 0.9f));

    /*m_camera.setToIdentity();
    m_camera.translate(0, 0, -m_camDistance);

    m_world.setToIdentity();
    m_world.rotate(m_camXRot, 1, 0, 0);
    m_world.rotate(m_camYRot, 0, 1, 0);
    m_world.rotate(m_camZRot, 0, 0, 1);*/

    m_camera.setToIdentity();
    m_world.setToIdentity();

    //FPP
        /*m_camera.lookAt(m_player.position,
                    m_player.position + m_player.direction,
                    QVector3D(0,1,0));

    //TPP
    if(cameraChoiceID == TPP){
    m_camera.lookAt(
        m_player.position - m_camDistance * m_player.direction,
        m_player.position,
        QVector3D(0, 1, 0) );
    }*/
    cameraTypeUpdateGL();
    //cout<<"dsajdsyahb"<<endl;
    for(int i= 0; i< m_gameObjects.size();i++){
        GameObject* obj = m_gameObjects[i];
        //cout<<"dupa";
        m_program->setUniformValue(m_modelColorLoc, obj->material_color);

        worldMatrixStack.push(m_world);
            m_world.translate(obj->position);
            m_world.rotate(obj->rotation.x(),1,0,0);
            m_world.rotate(obj->rotation.y(),0,1,0);
            m_world.rotate(obj->rotation.z(),0,0,1);
            m_world.scale(obj->scale);
            setTransforms();
            obj->render(this);
        m_world = worldMatrixStack.pop();
    }

    m_program->release();

	float timerTime = timer.elapsed() * 0.001f;
	float deltaTime = timerTime - lastUpdateTime;
	if (deltaTime >= (1.0f / FPS))
	{
		updateGL();
			lastUpdateTime = timerTime;
	}

    if(m_keyState[Qt::Key_Z]) m_camDistance += 0.005f;
    if(m_keyState[Qt::Key_X]) m_camDistance -= 0.005f;

    update();
}

void GLWidget::setTransforms(void)
{
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_viewMatrixLoc, m_camera);
    m_program->setUniformValue(m_modelMatrixLoc, m_world);
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - width()/2;
    int dy = event->y() - height()/2;
    //cout << "DX:  "<<dx<<"\tDY:  "<<dy<<endl;

    //for 3 types of camera
    if(cameraType==FPP || cameraType==TPP || cameraType==FPP_GOD){
        float phi = atan2(m_player.direction.z(), m_player.direction.x());
        float theta = acosf(m_player.direction.y());

        // obliczenie phi i theta
        phi = phi + dx * 0.01f;
        theta = theta + dy * 0.01f;

        if(theta < 0.01f) theta = 0.01f;
        if(theta > 3.14f) theta = 3.14f;

        // ustawienie m_player.direction
        m_player.direction.setX(sin(theta) * cos(phi));
        m_player.direction.setY(cos(theta));
        m_player.direction.setZ(sin(theta) * sin(phi));
    }

    //isimetric camera should have blocked camera rottion and rotate player around axis Y
    if(cameraType==ISOMETRIC){

    }

    /*if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_camXRot + 0.5f * dy);
        setYRotation(m_camYRot + 0.5f * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_camXRot + 0.5f * dy);
        setZRotation(m_camZRot + 0.5f * dx);
    }*/
    m_lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        exit(0);
    //Camera type change
    else if(e->key() == Qt::Key_F1)
        cameraType = FPP;
    else if(e->key() == Qt::Key_F2)
        cameraType = TPP;
    else if(e->key() == Qt::Key_F3)
        cameraType = ISOMETRIC;
    else if(e->key() == Qt::Key_F4)
        cameraType = TPP;
    //=======
    //shooting left mouse
    else if (e->key() == Qt::Key_Space){
        Bullet* bullet = new Bullet();

        bullet->position = m_player.position + m_player.direction * 0.7f;
        bullet->position.setY(0);
        bullet->scale = QVector3D(0.5f, 0.5f, 0.5f);
        bullet->m_radius = 0.5f;
        bullet->energy = 3 * m_player.direction;
        bullet->energy.setY(0);

        addObject(bullet);
    }
    else
        QWidget::keyPressEvent(e);

    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = false;
}

void GLWidget::keyboardAction(){
    if(m_keyState[Qt::Key_W])
    {
        m_player.energy.setX(m_player.energy.x() + m_player.direction.x() * m_player.speed);
        m_player.energy.setZ(m_player.energy.z() + m_player.direction.z() * m_player.speed);
    }
    if(m_keyState[Qt::Key_S]){
        m_player.energy.setX(m_player.energy.x() - m_player.direction.x() * m_player.speed);
        m_player.energy.setZ(m_player.energy.z() - m_player.direction.z() * m_player.speed);
    }
    if(m_keyState[Qt::Key_A]){
        m_player.energy.setX(m_player.energy.x() + m_player.direction.z() * m_player.speed);
        m_player.energy.setZ(m_player.energy.z() - m_player.direction.x() * m_player.speed);
    }
    if(m_keyState[Qt::Key_D]){
        m_player.energy.setX(m_player.energy.x() - m_player.direction.z() * m_player.speed);
        m_player.energy.setZ(m_player.energy.z() + m_player.direction.x() * m_player.speed);
    }
    if(m_keyState[Qt::Key_Q]){
        float phi = atan2(m_player.direction.z(), m_player.direction.x());
        phi -= 0.05f;
        m_player.direction.setX(cos(phi));
        m_player.direction.setZ(sin(phi));
    }
    if(m_keyState[Qt::Key_E]){
        float phi = atan2(m_player.direction.z(), m_player.direction.x());
        phi += 0.05f;
        m_player.direction.setX(cos(phi));
        m_player.direction.setZ(sin(phi));
    }
    if(m_keyState[Qt::Key_T]){
        float r = sqrtf(powf(m_player.direction.x(), 2)+ powf(m_player.direction.y(), 2)+ powf(m_player.direction.z(), 2));
        float phi = atan2(m_player.direction.z(), m_player.direction.x());
        float theta = acosf(m_player.direction.y()/ r);
        theta -=0.05f;
        m_player.direction.setX(r * sin(theta) * cos(phi));
        m_player.direction.setY(r * cos(theta));
        m_player.direction.setZ(r * sin(theta) * sin(phi));
    }
    if(m_keyState[Qt::Key_G]){
        float phi = atan2(m_player.direction.z(), m_player.direction.x());
        float theta = acosf(m_player.direction.y());
        theta +=0.05f;
        m_player.direction.setX(sin(theta) * cos(phi));
        m_player.direction.setY(cos(theta));
        m_player.direction.setZ(sin(theta) * sin(phi));
    }
    //m_player.printPosition();
    //m_player.printDirection();
}
void GLWidget::cameraTypeUpdateGL(){
    //FPP
    if(cameraType == FPP){
        m_camera.lookAt(m_player.position,
                    m_player.position + m_player.direction,
                    QVector3D(0,1,0));
    }
    //TPP
    else if(cameraType == TPP){
        m_camera.lookAt(
            m_player.position - m_camDistance * m_player.direction,
            m_player.position,
            QVector3D(0, 1, 0) );
    }
    else if(cameraType == ISOMETRIC){
//m_player.position - m_camDistance -
        m_camera.lookAt(
            QVector3D(m_player.position.x()+m_camDistance, m_camDistance*2, m_player.position.z() + m_camDistance),
            m_player.position,
            QVector3D(0, 1, 0) );
    }
    else if(cameraType == FPP_GOD){

    }
}

void GLWidget::addObject(GameObject* obj)
{
    obj->init();
    m_gameObjects.push_back(obj);
}

void GLWidget::updateGL()
{
    keyboardAction();
    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2))); //mysz na srodek
    //cout<<m_gameObjects.size()<<endl;
    for(int i = 0 ; i < m_gameObjects.size(); i++){
        GameObject* obj = m_gameObjects[i];

            // Porównujemy każdy obiekt z każdym
        for(int j = 0 ; j < m_gameObjects.size() ; j++)
        {
            // Nie porównujemy obiektów samych ze sobą
            if(i == j){
                continue;
            }
            GameObject* obj2 = m_gameObjects[j];

                // Liczymy wektor od pozycji jednego obiektu do drugiego
            QVector3D v = obj->position - obj2->position;
                // Długość tego wektora to odległość między środkami obiektów
            float d = v.length();
                // Porównujemy z sumą promieni
            if(d < (obj->m_radius + obj2->m_radius))
            {
             /*       // Reakcja na kolizję!
                v.normalize();
                float energySum = obj->energy.length() + obj2->energy.length();
                obj->energy = v * energySum / 2;
                obj2->energy = -v * energySum / 2;*/

                string name1 = obj->m_name;
                string name2 = obj2->m_name;

                GameObject* o1 = obj;
                GameObject* o2 = obj2;

                if(strcmp(name1.c_str(), name2.c_str()) > 0){
                    o1 = obj2;
                    o2 = obj;
                    v= -v;
                }
                if(!o1->m_name.compare("Player") && !o2->m_name.compare("bullet")){

                } else{
                    //Reakcja na kolizje
                    v.normalize();
                    float energySum = o1->energy.length() + o2->energy.length();
                    o1->energy = v * energySum / 2;
                    o2->energy = -v * energySum / 2;
                }

            }
        }
        obj->update();
    }

    //deleting bullets and if isAlive get false other object
    for(int i = 0 ; i < m_gameObjects.size(); ){
        GameObject* obj = m_gameObjects[i];
        if(obj->isAlive == false){
            m_gameObjects.erase(m_gameObjects.begin() + i);
            delete obj;
        }else
            i++;
    }


}
