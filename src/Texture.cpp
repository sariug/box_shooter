#include "Texture.h"

Texture::Texture(char *fileloc)
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    FileLocation = fileloc;
}

Texture::Texture()
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
}

void Texture::LoadTexture()
{
    unsigned char *texData = stbi_load(FileLocation, &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("No data is loaded\n");
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);

    textureID = 0;
    width = height = bitDepth = 0;
    FileLocation = "";
}

Texture::~Texture() { ClearTexture(); }