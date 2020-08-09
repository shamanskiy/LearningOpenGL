#pragma once

#include <GL/glew.h>

// Mesh is a class that contains information about an object to be rendred
class Mesh
{
private:
    // Vertex Array, Vertex Buffer and Element Buffer Objects
    // hold reference numbers to data objects on GPU
    GLuint VAO, VBO, EBO;
    // total number of indices to draw = num of elements * 3
    GLsizei idxCount;
    
public:
    // default constructor, does not initialize anything on GPU
    Mesh();
    ~Mesh();
    
    // create VAO, VBO, EBO on GPU
    void createMesh(GLfloat * vertices, unsigned int * indices,
                   unsigned int numVertices, unsigned int numIndices);
    // render object
    void renderMesh();
    // delete object from GPU memory
    void deleteMesh();
};


