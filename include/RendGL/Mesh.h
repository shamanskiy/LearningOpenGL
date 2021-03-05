#pragma once

#include <vector>

#include <GL/glew.h>

using namespace std;

// Mesh represents a single 3D object. It gets an array of vertices
// and an array of indices, copies them to the GPU and then holds
// the pointers to the arrays of the GPU.
class Mesh
{
public:
    Mesh(const vector<GLfloat> &  vertices,
        const vector<GLuint> & indices);
    ~Mesh();
    
    // render object
    void renderMesh() const;

private:
    // create arrays on the GPU
    void createMesh(const vector<GLfloat> & vertices,
        const vector<GLuint> & indices);

    // delete object from GPU memory
    void deleteMesh();

private:
    // Vertex Array, Vertex Buffer and Element Buffer Objects
    // hold reference numbers to data objects on GPU
    GLuint m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
    // total number of indices to draw = num of elements * 3
    GLuint m_numIndices{0};
};


