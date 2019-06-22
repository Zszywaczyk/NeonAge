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
            cube->material.diffuse.setX(i * 0.2f);
            cube->material.diffuse.setY(0.5f);
            cube->material.diffuse.setZ(j * 0.1f);
            // Wielkość Cube'a.
            // Ustawienie w jednej linijce
            // zamiast osobno dla X, Y i Z.
            cube->scale = QVector3D(0.3f, 0.3f, 0.3f);
            cube->m_radius = 0.5 * sqrt(3 * cube->scale.x() * cube->scale.x());
            cube->m_texture = TextureManager::getTexture("brick");
            // Dodanie obiektu do sceny.
            addObject(cube);
        }
    }
}

void GLWidget::addEnemy(){
    Enemy* cube = new Enemy();
    // Ustawienie pozycji Cube'a
    cube->position.setX(3);
    cube->position.setY(0);
    cube->position.setZ(3);
    // Kolor Cube'a. Zwykły gradient.
    cube->material.diffuse.setX(0.2f);
    cube->material.diffuse.setY(0.5f);
    cube->material.diffuse.setZ(0.1f);
    // Wielkość Cube'a.
    // Ustawienie w jednej linijce
    // zamiast osobno dla X, Y i Z.
    cube->scale = QVector3D(1.0f, 1.0f, 1.0f);
    cube->m_radius = 0.5 * sqrt(3 * cube->scale.x() * cube->scale.x());
    cube->m_texture = TextureManager::getTexture("enemy");
    // Dodanie obiektu do sceny.
    addObject(cube);

    cube = new Enemy();
        // Ustawienie pozycji Cube'a
        cube->position.setX(-3);
        cube->position.setY(0);
        cube->position.setZ(-3);
        // Kolor Cube'a. Zwykły gradient.
        cube->material.diffuse.setX(0.2f);
        cube->material.diffuse.setY(0.5f);
        cube->material.diffuse.setZ(0.1f);
        // Wielkość Cube'a.
        // Ustawienie w jednej linijce
        // zamiast osobno dla X, Y i Z.
        cube->scale = QVector3D(1.0f, 1.0f, 1.0f);
        cube->m_radius = 0.5 * sqrt(3 * cube->scale.x() * cube->scale.x());
        cube->m_texture = TextureManager::getTexture("enemy");
        // Dodanie obiektu do sceny.
        addObject(cube);

        cube = new Enemy();
            // Ustawienie pozycji Cube'a
            cube->position.setX(9);
            cube->position.setY(0);
            cube->position.setZ(9);
            // Kolor Cube'a. Zwykły gradient.
            cube->material.diffuse.setX(0.2f);
            cube->material.diffuse.setY(0.5f);
            cube->material.diffuse.setZ(0.1f);
            // Wielkość Cube'a.
            // Ustawienie w jednej linijce
            // zamiast osobno dla X, Y i Z.
            cube->scale = QVector3D(1.0f, 1.0f, 1.0f);
            cube->m_radius = 0.5 * sqrt(3 * cube->scale.x() * cube->scale.x());
            cube->m_texture = TextureManager::getTexture("enemy");
            // Dodanie obiektu do sceny.
            addObject(cube);

            cube = new Enemy();
                // Ustawienie pozycji Cube'a
                cube->position.setX(-9);
                cube->position.setY(0);
                cube->position.setZ(-9);
                // Kolor Cube'a. Zwykły gradient.
                cube->material.diffuse.setX(0.2f);
                cube->material.diffuse.setY(0.5f);
                cube->material.diffuse.setZ(0.1f);
                // Wielkość Cube'a.
                // Ustawienie w jednej linijce
                // zamiast osobno dla X, Y i Z.
                cube->scale = QVector3D(1.0f, 1.0f, 1.0f);
                cube->m_radius = 0.5 * sqrt(3 * cube->scale.x() * cube->scale.x());
                cube->m_texture = TextureManager::getTexture("enemy");
                // Dodanie obiektu do sceny.
                addObject(cube);
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
    m_program->bindAttributeLocation("uvCoord", 2);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program->uniformLocation("viewMatrix");
    m_modelMatrixLoc = m_program->uniformLocation("modelMatrix");
    m_hasTextureLoc = m_program->uniformLocation("hasTexture");
    m_cameraPositionLoc = m_program->uniformLocation("cameraPosition");

    m_materialLoc.ambient = m_program->uniformLocation("material.ambient");
    m_materialLoc.diffuse = m_program->uniformLocation("material.diffuse");
    m_materialLoc.specular = m_program->uniformLocation("material.specular");
    m_materialLoc.shininess = m_program->uniformLocation("material.shininess");

    for(int i = 0; i < MAX_LIGHTS; i++){
        m_lightLoc[i].position = m_program->uniformLocation(QString().asprintf("light[%d].position", i));
        m_lightLoc[i].ambient = m_program->uniformLocation(QString().asprintf("light[%d].ambient", i));
        m_lightLoc[i].diffuse = m_program->uniformLocation(QString().asprintf("light[%d].diffuse", i));
        m_lightLoc[i].specular = m_program->uniformLocation(QString().asprintf("light[%d].specular", i));
        m_lightLoc[i].isActive = m_program->uniformLocation(QString().asprintf("light[%d].isActive", i));
        m_lightLoc[i].attenuation = m_program->uniformLocation(QString().asprintf("light[%d].attenuation", i));
    }

    m_program->release();

    m_program_hud = new QOpenGLShaderProgram;
    m_program_hud->addShaderFromSourceFile(QOpenGLShader::Vertex, "resources/shader_hud.vs");
    m_program_hud->addShaderFromSourceFile(QOpenGLShader::Fragment, "resources/shader_hud.fs");
    m_program_hud->bindAttributeLocation("vertex", 0);
    m_program_hud->bindAttributeLocation("normal", 1);
    m_program_hud->bindAttributeLocation("uvCoord", 2);
    m_program_hud->link();

    m_program_hud->bind();

    m_resolutionLoc_hud = m_program_hud->uniformLocation("resolution");
    m_color_hud = m_program_hud->uniformLocation("color");
    m_hasTextureLoc_hud = m_program_hud->uniformLocation("hasTexture");
    m_rectangleLoc_hud.xPos = m_program_hud->uniformLocation("rect.xPos");
    m_rectangleLoc_hud.yPos = m_program_hud->uniformLocation("rect.yPos");
    m_rectangleLoc_hud.width = m_program_hud->uniformLocation("rect.width");
    m_rectangleLoc_hud.height = m_program_hud->uniformLocation("rect.height");

    m_program_hud->release();
	
	lastUpdateTime = 0;
    timer.start();

    CMesh::loadAllMeshes();
    TextureManager::init();
    initCollisionTriangles();
    addObject(&m_player);
    gridOfCubes();
    addEnemy();
	
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QStack<QMatrix4x4> worldMatrixStack;

    m_lights[0].position = QVector3D(-3.0f, 1.0f, -4.0f);
    m_lights[0].ambient = QVector3D(0.1f, 0.1f, 0.1f);
    m_lights[0].diffuse = QVector3D(1.0f, 0.3f, 0.3f);
    m_lights[0].specular = QVector3D(0.1f, 0.1f, 0.1f);
    m_lights[0].isActive = true;
    m_lights[0].attenuation = 0.5f;

    m_lights[1].position = QVector3D(3.0f, 1.0f, -4.0f);
    m_lights[1].ambient = QVector3D(0.1f, 0.1f, 0.1f);
    m_lights[1].diffuse = QVector3D(0.3f, 1.0f, 0.3f);
    m_lights[1].specular = QVector3D(0.1f, 0.1f, 0.1f);
    m_lights[1].isActive = true;
    m_lights[1].attenuation = 0.5f;

    m_program->bind();
    m_program->setUniformValue(m_cameraPositionLoc, m_player.position - m_camDistance * m_player.direction);
    setLights();

    m_world.setToIdentity();
    m_camera.setToIdentity(); 

    cameraTypeUpdateGL();

    for(int i= 0; i< m_gameObjects.size();i++){
        GameObject* obj = m_gameObjects[i];

        m_program->setUniformValue(m_materialLoc.ambient, obj->material.ambient);
        m_program->setUniformValue(m_materialLoc.diffuse, obj->material.diffuse);
        m_program->setUniformValue(m_materialLoc.specular, obj->material.specular);
        m_program->setUniformValue(m_materialLoc.shininess, obj->material.shininess);

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
    for(int i = 0; i < collisionTriangles.size();/*nic*/){
        Triangle triangle = collisionTriangles[i];

        m_program->setUniformValue(m_materialLoc.ambient, QVector3D(1.0f,1.0f,1.0f));
        m_program->setUniformValue(m_materialLoc.diffuse, QVector3D(1.0f,1.0f,1.0f));
        m_program->setUniformValue(m_materialLoc.specular, QVector3D(1.0f,1.0f,1.0f));
        m_program->setUniformValue(m_materialLoc.shininess, 1.0f);

        if(triangle.texture != nullptr){
            m_program->setUniformValue(m_hasTextureLoc, 1);
            triangle.texture->bind();
        }else{
            m_program->setUniformValue(m_hasTextureLoc, 0);
        }

        worldMatrixStack.push(m_world);
            m_world.translate(QVector3D(0,0,0));
            m_world.rotate(0,1,0,0);
            m_world.rotate(0,0,1,0);
            m_world.rotate(0,0,0,1);
            m_world.scale(QVector3D(1,1,1));
            setTransforms();
            collisionTrianglesMesh.render(this, i * 3, triangle.groupSize * 3);
        m_world = worldMatrixStack.pop();

        i+= triangle.groupSize;
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

    paintHUD();

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

    m_resolution = QVector2D(w, h);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();

     if (event->buttons() == Qt::LeftButton && started== true){
        Bullet* bullet = new Bullet();

        bullet->position = m_player.position + m_player.direction * 0.7f;
        bullet->position.setY(0);
        bullet->scale = QVector3D(0.5f, 0.5f, 0.5f);
        bullet->m_radius = 0.5f;
        bullet->energy = 3 * m_player.direction;
        bullet->energy.setY(0);

        addObject(bullet);
    }

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
    else if (e->key() == Qt::Key_Space && started== true){
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

    if(started==false && m_keyState[Qt::Key_W]){
            started=true;
    }

    if(m_player.hp<=0 && m_keyState[Qt::Key_W]){
        //GLWidget glwidget = new GL
        m_player.hp=m_player.MAX_HP;
        started= false;
        isWinner=false;
        gridOfCubes();
        paintGL();
    }

    if(m_player.hp>0 && m_keyState[Qt::Key_W] && isWinner==true){
        //GLWidget glwidget = new GL
        m_player.hp=m_player.MAX_HP;
        started= false;
        isWinner=false;
        gridOfCubes();
        paintGL();
    }

    //m_player.printPosition();
    //m_player.printDirection();
}
void GLWidget::cameraTypeUpdateGL(){
    //FPP
    if(cameraType == FPP){
        m_camera.lookAt(m_player.position,
                    m_player.position + m_player.direction,
                    QVector3D(0,10,0));
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
    /*if( ((timer.elapsed()*0.001)%10) ==0 ){
        m_player.hp -=1;
    }*/

    int isAnyCube=0;

        //cout << timer.elapsed()*0.001<<endl<<endl;
        QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2))); //mysz na srodek
        //cout<<m_gameObjects.size()<<endl;
        for(int i = 0 ; i < m_gameObjects.size(); i++){
            GameObject* obj = m_gameObjects[i];

            if(obj->m_name.compare("cube")==0 && obj->position.y()<0){
                obj->position.setY(1.0);
            }

            if(obj->m_name.compare("cube")==0){
                isAnyCube++;
            }
            if(started){
            if(obj->m_name.compare("Enemy") == 0){
                if(m_player.position.x() > obj->position.x()){
                    obj->position.setX(obj->position.x() + m_player.speed*2);
                } else {
                    obj->position.setX(obj->position.x() - m_player.speed*2);
                }

                if(m_player.position.y() > obj->position.y()){
                    obj->position.setY(obj->position.y() + m_player.speed*2);
                } else {
                    obj->position.setY(obj->position.y() - m_player.speed*2);
                }

                if(m_player.position.z() > obj->position.z()){
                    obj->position.setZ(obj->position.z() + m_player.speed*2);
                } else {
                    obj->position.setZ(obj->position.z() - m_player.speed*2);
                }
            }
            }

            obj->previousPosition = obj->position;  // Poprzednia pozycja

                // Porównujemy każdy obiekt z każdym
            for(int j = 0 ; j < m_gameObjects.size() ; j++)
            {
                // Nie porównujemy obiektów samych ze sobą
                if(i == j){
                    continue;
                }
                GameObject* obj2 = m_gameObjects[j];
                if(obj2->m_name.compare("cube")==0){
                    isAnyCube++;
                }
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
                    name1 = o1->m_name;
                    name2 = o2->m_name;

                    if((strcmp(name1.c_str(),"Player") && strcmp(name2.c_str(),"Enemy")) && (strcmp(name1.c_str(),"cube") && strcmp(name2.c_str(),"cube")) && (strcmp(name1.c_str(),"bullet") && strcmp(name2.c_str(),"bullet")) ){
                        m_player.hp-=0.5f;
                        //o2->isAlive=false;
                        cout << o1->m_name.compare("Player") <<" "<< o1->m_name<< "    " <<o2->m_name.compare("Enemy")<<" "<<o2->m_name<<endl;
                    }

                    if((strcmp(name1.c_str(),"Player") && strcmp(name2.c_str(),"Enemy")) && (strcmp(name1.c_str(),"cube") && strcmp(name2.c_str(),"cube")) && (strcmp(name1.c_str(),"bullet") && strcmp(name2.c_str(),"bullet")) ){
                        m_player.hp-=0.5f;
                        //o2->isAlive=false;
                        cout << o1->m_name.compare("Player") <<" "<< o1->m_name<< "    " <<o2->m_name.compare("Enemy")<<" "<<o2->m_name<<endl;
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

        if(isAnyCube<=0&&m_player.hp>0){
            isWinner= true;
            started = false;
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

    /*addTriangleCollider(
                QVector3D(25,0,-25),
                QVector3D(-25,0,-25),
                QVector3D(25,0,25),
                1,
                QVector2D(1,1),
                QVector2D(0,1),
                QVector2D(1,0),
                TextureManager::getTexture("grass"));*/

    /*addTriangleCollider(
                QVector3D(-25,0,-25),
                QVector3D(-25,0,25),
                QVector3D(25,0,25),
                1,
                QVector2D(0,1),
                QVector2D(0,0),
                QVector2D(1,0),
                TextureManager::getTexture("grass"));*/

    samplescene();

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

void GLWidget::samplescene(){
    addTriangleCollider(QVector3D(30, 0, -30), QVector3D(-30, 0, -30), QVector3D(30, 0, 30), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("grass"));
    addTriangleCollider(QVector3D(-30, 0, -30), QVector3D(-30, 0, 30), QVector3D(30, 0, 30), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("grass"));

    addTriangleCollider(QVector3D(-15, 0, 15), QVector3D(-15, 15, 15), QVector3D(15, 15, 15), 1, QVector2D(1, 0), QVector2D(1, 1), QVector2D(0, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(15, 15, 15), QVector3D(15, 0, 15), QVector3D(-15, 0, 15), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-15, 15, 15), QVector3D(-15, 0, 15), QVector3D(-15, 15, 20), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-15, 0, 15), QVector3D(-15, 0, 20), QVector3D(-15, 15, 20), 1, QVector2D(0, 0), QVector2D(1, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-35, 0, 20), QVector3D(-35, 15, 20), QVector3D(-15, 15, 20), 1, QVector2D(1, 0), QVector2D(1, 1), QVector2D(0, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-15, 15, 20), QVector3D(-15, 0, 20), QVector3D(-35, 0, 20), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-35, 15, 20), QVector3D(-35, 0, 20), QVector3D(-35, 15, -30), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-35, 0, 20), QVector3D(-35, 0, -30), QVector3D(-35, 15, -30), 1, QVector2D(0, 0), QVector2D(1, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(25, 0, -30), QVector3D(25, 15, -30), QVector3D(-35, 15, -30), 1, QVector2D(1, 0), QVector2D(1, 1), QVector2D(0, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(-35, 15, -30), QVector3D(-35, 0, -30), QVector3D(25, 0, -30), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(25, 15, -30), QVector3D(25, 0, -30), QVector3D(25, 15, 25), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(25, 0, -30), QVector3D(25, 0, 25), QVector3D(25, 15, 25), 1, QVector2D(0, 0), QVector2D(1, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(25, 15, 25), QVector3D(25, 0, 25), QVector3D(15, 15, 15), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));
    addTriangleCollider(QVector3D(25, 0, 25), QVector3D(15, 0, 15), QVector3D(15, 15, 15), 1, QVector2D(0, 0), QVector2D(1, 0), QVector2D(1, 1), TextureManager::getTexture("brick"));

    addTriangleCollider(QVector3D(5, 3, -21), QVector3D(-5, 3, -21), QVector3D(5, 3, -13), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -21), QVector3D(-5, 3, -13), QVector3D(5, 3, -13), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -21), QVector3D(-10, 0, -21), QVector3D(-5, 3, -13), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-10, 0, -21), QVector3D(-10, 0, -13), QVector3D(-5, 3, -13), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(10, 0, -21), QVector3D(5, 3, -21), QVector3D(10, 0, -13), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(5, 3, -21), QVector3D(5, 3, -13), QVector3D(10, 0, -13), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(5, 3, -13), QVector3D(-5, 3, -13), QVector3D(5, 0, -13), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -13), QVector3D(-5, 0, -13), QVector3D(5, 0, -13), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -13), QVector3D(-10, 0, -13), QVector3D(-5, 0, -13), 1, QVector2D(1, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(5, 3, -13), QVector3D(5, 0, -13), QVector3D(10, 0, -13), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -21), QVector3D(5, 3, -21), QVector3D(-5, 0, -21), 1, QVector2D(1, 1), QVector2D(0, 1), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(5, 3, -21), QVector3D(5, 0, -21), QVector3D(-5, 0, -21), 1, QVector2D(0, 1), QVector2D(0, 0), QVector2D(1, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(-5, 3, -21), QVector3D(-5, 0, -21), QVector3D(-10, 0, -21), 1, QVector2D(1, 1), QVector2D(1, 0), QVector2D(0, 0), TextureManager::getTexture("wood"));
    addTriangleCollider(QVector3D(5, 3, -21), QVector3D(10, 0, -21), QVector3D(5, 0, -21), 1, QVector2D(0, 1), QVector2D(1, 0), QVector2D(0, 0), TextureManager::getTexture("wood"));
}

void GLWidget::setLights(){
    for(int i=0; i<MAX_LIGHTS; i++){
        m_program->setUniformValue(m_lightLoc[i].position, m_lights[i].position);
        m_program->setUniformValue(m_lightLoc[i].ambient, m_lights[i].ambient);
        m_program->setUniformValue(m_lightLoc[i].diffuse, m_lights[i].diffuse);
        m_program->setUniformValue(m_lightLoc[i].specular, m_lights[i].specular);
        m_program->setUniformValue(m_lightLoc[i].isActive, m_lights[i].isActive);
        m_program->setUniformValue(m_lightLoc[i].attenuation, m_lights[i].attenuation);
    }
}

void GLWidget::setRectangle(float xPos, float yPos, float width, float height, QVector3D color, QOpenGLTexture *texture){
    m_program_hud->setUniformValue(m_rectangleLoc_hud.xPos, xPos);
    m_program_hud->setUniformValue(m_rectangleLoc_hud.yPos, yPos);
    m_program_hud->setUniformValue(m_rectangleLoc_hud.width, width);
    m_program_hud->setUniformValue(m_rectangleLoc_hud.height, height);
    m_program_hud->setUniformValue(m_color_hud, color);

    if(texture != nullptr){
        m_program_hud->setUniformValue(m_hasTextureLoc_hud, 1);
        texture->bind();
    }
    else{
        m_program_hud->setUniformValue(m_hasTextureLoc_hud, 0);
    }
}

void GLWidget::paintHUD(){
    CMesh* rectMesh = CMesh::m_meshes["rect"];
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program_hud->bind();
    m_program_hud->setUniformValue(m_resolutionLoc_hud, m_resolution);

    // Elementy HUDu
    //setRectangle(30,30,200,100,QVector3D(1,1,0), nullptr);
    //rectMesh->render(this);

    if(started == false && m_player.hp>0 && isWinner==false){
        QPainter painter(this);
        painter.setPen(QColor(255.0f, 255.0f, 255.0f, 255.0f));
        painter.setFont(QFont("Helvetica", 26));
        painter.drawText(width()/2, height()/2, "Wcisnij klawisz 'W' aby zacząć");
        painter.end();
        setRectangle(0, 0, width(), height(), QVector3D(0.0,0.0,0.0), nullptr);
        rectMesh->render(this);
    }

    if(started == false && m_player.hp>0 && isWinner==true){
        QPainter painter(this);
        painter.setPen(QColor(255.0f, 255.0f, 255.0f, 255.0f));
        painter.setFont(QFont("Helvetica", 26));
        painter.drawText(width()/2, height()/2, "Gratulacje, udało ci się");
        painter.end();
        setRectangle(0, 0, width(), height(), QVector3D(0.0,0.0,0.0), nullptr);
        rectMesh->render(this);
    }

    if(m_player.hp<=0){
        started = false;
        QPainter painter(this);
        painter.setPen(QColor(255.0f, 255.0f, 255.0f, 255.0f));
        painter.setFont(QFont("Helvetica", 26));
        painter.drawText(width()/2, height()/2, "Przegrałeś. Spróbuj jeszcze raz");
        painter.end();
        setRectangle(0, 0, width(), height(), QVector3D(0.0,0.0,0.0), nullptr);
        rectMesh->render(this);

    }

    float playerHP = m_player.hp;
    float playerHPMax = m_player.MAX_HP;
    float posX = 20;
    float posY = 20;
    float width = 250;
    float height = 10;
    float frame = 2;

    setRectangle(posX, posY+frame, width, height/5, QVector3D(0.9176,0,0.8509), nullptr);
    rectMesh->render(this);

    setRectangle(posX, posY, width*playerHP/playerHPMax, height, QVector3D(0.0392,0.7411,0.7764), nullptr);
    rectMesh->render(this);


    //setRectangle(posX+width*playerHP/playerHPMax, posY, width*(1-playerHP/playerHPMax), height, QVector3D(0,0,0), nullptr);
    //rectMesh->render(this);

    //setRectangle(posX-frame, posY-frame, width+frame*2, height+frame*2, QVector3D(0,0.5,0), nullptr);
    //rectMesh->render(this);

    //==============

    glDisable(GL_BLEND);
    m_program_hud->release();
}
