

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
    // ctor: create texture on the GPU
    Texture(const std::string & fileLocation);
    // dtor: free memory on GPU
    ~Texture();
    // activate/bind texture on the GPU
    void useTexture() const;
    
private:
    // load texture from file
    void loadTexture();
    // free memory on the GPU
    void clearTexture();
};
