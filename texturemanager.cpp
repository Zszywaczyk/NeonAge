#include "texturemanager.h"

map<string, QOpenGLTexture*> TextureManager::m_textures;

TextureManager::TextureManager()
{

}

void TextureManager::init(){
    m_textures["brick"] = new QOpenGLTexture(QImage("resources/brick.jpg"));
}

QOpenGLTexture* TextureManager::getTexture(string name){
    return m_textures[name];
}
