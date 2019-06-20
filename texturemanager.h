#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
using namespace std;

#include <map>
#include <string>
#include <QOpenGLTexture>

class TextureManager
{
public:
    TextureManager();

    static void init();

    static map<string, QOpenGLTexture*> m_textures;

    static QOpenGLTexture* getTexture(string name);
};

#endif // TEXTUREMANAGER_H
