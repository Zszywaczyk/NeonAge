#include "texturemanager.h"

map<string, QOpenGLTexture*> TextureManager::m_textures;

TextureManager::TextureManager()
{

}

void TextureManager::init(){
    m_textures["brick"] = new QOpenGLTexture(QImage("resources/brick.jpg"));
    m_textures["grass"] = new QOpenGLTexture(QImage("resources/grass.jpg"));
    m_textures["wood"] = new QOpenGLTexture(QImage("resources/wood.jpg"));
    m_textures["enemy"] = new QOpenGLTexture(QImage("resources/enemy.png"));
}

QOpenGLTexture* TextureManager::getTexture(string name){
    return m_textures[name];
}
