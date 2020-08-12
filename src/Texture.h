#pragma once

#include <string>

#include <GL/glew.h>
// must be here for some reason; can't move to Texture.cpp
#include <stb_image.h>

// Texture is a class for loading and managing textures
class Texture
{
private:
    // ID of the texture object on the GPU
    GLuint textureID;
    // texture image properties
    int width, height, bitDepth;
    // location of the texture file
    std::string fileLoc;

public:
    // ctors and dtor
    Texture();
    Texture(const std::string & fileLocation);
    ~Texture();

    // load texture from file
    void loadTexture();
    // activate/bind texture on the GPU
    void useTexture();
    // free memory on the GPU
    void clearTexture();
};
