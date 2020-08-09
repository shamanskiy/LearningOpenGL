#pragma once

#include <GL/glew.h>
#include <stb_image.h>

class Texture
{
private:
    GLuint textureID;
    int width, height, bitDepth;
    char* fileLoc;
    
public:
    Texture(char* fileLocation);
    ~Texture();
    
    void loadTexture();
    void useTexture();
    void clearTexture();
};
