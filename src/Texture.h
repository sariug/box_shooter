#pragma once
#include <GL/glew.h>
#include "../external/stb_image.h"

class Texture
{
public:
    Texture();
    Texture(char *fileloc);
    ~Texture();

    void LoadTexture();
    void ClearTexture();
    void UseTexture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    char *FileLocation;
};