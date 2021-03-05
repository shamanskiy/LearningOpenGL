#pragma once

#include <GL/glew.h>

// Mesh represents a single 3D object. It gets an array of vertices
// and an array of indices, copies them to the GPU and then holds
// the pointers to the arrays of the GPU.
class Mesh
{
public:
    Mesh(GLfloat* vertices, unsigned int* indices,
        unsigned int numVertices, unsigned int numIndices);
    ~Mesh();
    
    // render object
    void renderMesh() const;

private:
    // create arrays on the GPU
    void createMesh(GLfloat * vertices, unsigned int * indices,
                   unsigned int numVertices);

    // delete object from GPU memory
    void deleteMesh();

private:
    // Vertex Array, Vertex Buffer and Element Buffer Objects
    // hold reference numbers to data objects on GPU
    GLuint m_VAO, m_VBO, m_EBO;
    // total number of indices to draw = num of elements * 3
    GLsizei m_numIndices;
};


