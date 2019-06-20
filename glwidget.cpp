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
            cube->m_texture = TextureManager::getTexture("brick");
            // Dodanie obiektu do sceny.
            addObject(cube);
        }
    }
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.2f, 0.3f, 1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

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
    m_hasTextureLoc = m_program->uniformLocation("hasTexture");
    m_lightLoc.position = m_program->uniformLocation("light.position");
    m_lightLoc.ambient = m_program->uniformLocation("light.ambient");
    m_lightLoc.diffuse = m_program->uniformLocation("light.diffuse");

    m_program->release();
	
	lastUpdateTime = 0;
    timer.start();

    CMesh::loadAllMeshes();
    TextureManager::init();
    initCollisionTriangles();
    addObject(&m_player);
    gridOfCubes();
	
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

    m_camera.setToIdentity();
    m_world.setToIdentity();

    cameraTypeUpdateGL();

    for(int i= 0; i< m_gameObjects.size();i++){
        GameObject* obj = m_gameObjects[i];

        m_program->setUniformValue(m_modelColorLoc, obj->material_color);

        if(obj->m_texture != nullptr){
            m_program->setUniformValue(m_hasTextureLoc, 1);
            obj->m_texture->bind();
        }
        else{
            m_program->setUniformValue(m_hasTextureLoc, 0);
        }

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

        obj->previousPosition = obj->position;  // Poprzednia pozycja

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

                } else if(!o1->m_name.compare("bullet") && !o2->m_name.compare("cube")){
                    o1->isAlive = false;
                    o2->isAlive = false;
                } else{
                    //Reakcja na kolizje
                    o1->position = o1->position + v * (d/2); // Poprawa
                    o2->position = o2->position - v * (d/2); // kolizji
                    v.normalize();
                    float energySum = o1->energy.length() + o2->energy.length();
                    o1->energy = v * energySum / 2;
                    o2->energy = -v * energySum / 2;
                }

            }
        }
        obj->energy.setY(obj->energy.y() - 0.02f); //Grawitacja

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

    for(unsigned int i = 0; i< m_gameObjects.size();i++){
        GameObject* obj = m_gameObjects[i];

        for(int j = 0; j<collisionTriangles.size(); j++){
            Triangle tr = collisionTriangles[j];

            float currDist = tr.A * obj->position.x() + tr.B * obj->position.y() + tr.C * obj->position.z() + tr.D;
            float prevDist = tr.A * obj->previousPosition.x() + tr.B * obj->previousPosition.y() + tr.C * obj->previousPosition.z() + tr.D;

            if((currDist * prevDist < 0) || abs(currDist) < obj->m_radius){
                // Rzut pozycji obiektu na plaszczyznie
                QVector3D p = obj->position - tr.n*currDist;

                //Przesuniecie punktu do srodka trojkata o dlugosci promienia kolidera
                QVector3D r = (tr.v1 + tr.v2 + tr.v3)*(1.0f / 3.0f) - p;
                r = r.normalized();
                p = p+r*obj->m_radius;

                //Obliczenie v, w, u - wspolrzednych barycentrycznych
                QVector3D v0 = tr.v2 - tr.v1, v1 = tr.v3 - tr.v1, v2 = p - tr.v1;
                float d00 = QVector3D::dotProduct(v0, v0);
                float d01 = QVector3D::dotProduct(v0, v1);
                float d11 = QVector3D::dotProduct(v1, v1);
                float d20 = QVector3D::dotProduct(v2, v0);
                float d21 = QVector3D::dotProduct(v2, v1);
                float denom = d00 * d11 - d01 * d01;

                float v = (d11 * d20 - d01 * d21) / denom;
                float w = (d00 * d21 - d01 * d20) / denom;
                float u = 1.0f - v - w;

                if(v>=0 && w>=0 && (v+w)<=1){
                    float d = obj->m_radius - currDist;

                    obj->position = obj->position + tr.n * d;

                    obj->energy = obj->energy - tr.n * QVector3D::dotProduct(tr.n, obj->energy)*2;
                }

            }

        }

    }


}

void GLWidget::initCollisionTriangles(){

    //TUTAJ DODAWAĆ NOWE KOLIDERY start

    addTriangleCollider(
                QVector3D(25,0,-25),
                QVector3D(-25,0,-25),
                QVector3D(25,0,25),
                1,
                QVector2D(1,1),
                QVector2D(0,1),
                QVector2D(1,0),
                TextureManager::getTexture("grass"));

    addTriangleCollider(
                QVector3D(-25,0,-25),
                QVector3D(-25,0,25),
                QVector3D(25,0,25),
                1,
                QVector2D(0,1),
                QVector2D(0,0),
                QVector2D(1,0),
                TextureManager::getTexture("grass"));

    //TUTAJ DODAWAĆ NOWE KOLIDERY end

    collisionTrianglesMesh.m_primitive = GL_TRIANGLES;
    collisionTrianglesMesh.initVboAndVao();

}

void GLWidget::addTriangleCollider(QVector3D v1, QVector3D v2, QVector3D v3,
                                   int groupSize, QVector2D uv1, QVector2D uv2,
                                   QVector2D uv3, QOpenGLTexture *texture){

    Triangle t;
    t.v1 = v1;
    t.v2 = v2;
    t.v3 = v3;
    t.texture = texture;
    t.groupSize = groupSize;

    t.n = QVector3D::crossProduct(v1-v3, v2-v1).normalized();

    t.A = t.n.x();
    t.B = t.n.y();
    t.C = t.n.z();
    t.D = -(t.A * v1.x() + t.B * v1.y() + t.C*v1.z());

    collisionTriangles.push_back(t);

    collisionTrianglesMesh.add(t.v1, t.n, uv1);
    collisionTrianglesMesh.add(t.v2, t.n, uv2);
    collisionTrianglesMesh.add(t.v3, t.n, uv3);

}
