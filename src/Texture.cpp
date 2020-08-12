#include "Texture.h"

#include <iostream>

Texture::Texture() :
    textureID(0),
    width(0),
    height(0),
    bitDepth(0),
    fileLoc("")
{}

Texture::Texture(const std::string & fileLocation) :
    textureID(0),
    width(0),
    height(0),
    bitDepth(0),
    fileLoc(fileLocation)
{}

Texture::~Texture()
{
    clearTexture();
}

void Texture::loadTexture()
{
    // load image/texture data
    unsigned char *textureData = stbi_load(fileLoc.c_str(), &width, &height, &bitDepth, 0);
    if (!textureData)
        std::cout << "Failed to find a texture in " << fileLoc << std::endl;
    // create texture object on the GPU
    glGenTextures(1, &textureID);
    // activate/bind texture object for future operations
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x-wrap option (s is x for textures) GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y-wrap option (t is y for textures)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // texture scaling on zoom-out (related to mipmaps?) GL_LINEAR or GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // texture scaling on zoom-in (related to mipmaps?)

    glTexImage2D(GL_TEXTURE_2D, 0, // mipmap level
                 GL_RGBA, width, height, // original image properties
                  0, // this zero is a legacy parameter for handling borders
                 GL_RGBA, // image format again (why?)
                 GL_UNSIGNED_BYTE, textureData); // data type and data itself
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
}

void Texture::useTexture()
{
    // texture unit - this guy will access texture data. 0 is default, so this is line is not necessary.
    // by using several different texture units we can bind several textures (?)
    glActiveTexture(GL_TEXTURE0);

    // bind this texture to the texture unit 0
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
}
