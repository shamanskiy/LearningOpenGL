#include "Primitive.h"
#include <iostream>
#include "Mesh.h"
#include <glm/glm.hpp>

namespace
{

// compute normals at each vertex by averaging normals of every triangle
// which the vertex belongs to, i.e. Phong normals
void computeNormals(GLfloat * vertices, unsigned int numVertices, unsigned int * elements, unsigned int numElements)
{
    // compute normals for each triangle and add them to corresponding vertices
    for (int i = 0; i < numElements; ++i)
    {
        // 3 vertices of a given element
        int A = elements[i*3];
        int B = elements[i*3+1];
        int C = elements[i*3+2];
        glm::vec3 AB(vertices[B*8] - vertices[A*8], vertices[B*8 + 1] - vertices[A*8 + 1], vertices[B*8 + 2] - vertices[A*8 + 2] );
        glm::vec3 AC(vertices[C*8] - vertices[A*8], vertices[C*8 + 1] - vertices[A*8 + 1], vertices[C*8 + 2] - vertices[A*8 + 2] );
        
        // normalize the normal before adding
        glm::vec3 ABxAC = glm::normalize(glm::cross(AB, AC));
        vertices[A*8+5] += ABxAC.x; vertices[B*8+5] += ABxAC.x; vertices[C*8+5] += ABxAC.x;
        vertices[A*8+6] += ABxAC.y; vertices[B*8+6] += ABxAC.y; vertices[C*8+6] += ABxAC.y;
        vertices[A*8+7] += ABxAC.z; vertices[B*8+7] += ABxAC.z; vertices[C*8+7] += ABxAC.z;
    }
    
    for (int i = 0; i < numVertices; ++i)
    {
        glm::vec3 norm(vertices[i*8+5],vertices[i*8+6],vertices[i*8+7]);
        norm = glm::normalize(norm);
        vertices[i*8+5] = norm.x;
        vertices[i*8+6] = norm.y;
        vertices[i*8+7] = norm.z;
    }
}

} // anonymous namespace ends

std::unique_ptr<Mesh> makePlane()
{
    // 3 coordinates in 3D space, 2 texture coordinates, 3 normals.
    // normals are initialized with zeros and are computed later.
    GLfloat vertices[] = {
        // x     y     z        u      v      nx    ny    nz
        -0.5f, 0.0f, -0.5f,   0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  10.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 10.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  10.0f, 10.0f,  0.0f, 0.0f, 0.0f
    };
    
    unsigned int elements[] = {
        0, 2, 1,
        1, 2, 3
    };

    computeNormals(vertices, 4, elements, 2);
    auto plane = std::make_unique<Mesh>();
    plane->createMesh(vertices, elements, 32, 6);
    return plane;
}

std::unique_ptr<Mesh> makeCube()
{
    // 3 coordinates in 3D space, 2 texture coordinates, 3 normals.
    // normals are initialized with zeros and are computed later.
    GLfloat vertices[] = {
        // x      y      z      u     v      nx    ny    nz
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    unsigned int elements[] = {
        0, 2, 1,
        1, 2, 3,
        1, 3, 7,
        5, 1, 7,
        4, 5, 7,
        4, 7, 6,
        0, 4, 2,
        6, 2, 4
    };

    computeNormals(vertices, 8,elements, 8);
    auto cube = std::make_unique<Mesh>();
    cube->createMesh(vertices, elements, 64, 24);
    return cube;
}

std::unique_ptr<Mesh> makePyramid()
{
    // 3 coordinates in 3D space, 2 texture coordinates, 3 normals.
    // normals are initialized with zeros and are computed later.
    GLfloat vertices[] = {
        // x     y      z      u     v      nx    ny    nz
        -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f, 0.5f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    unsigned int elements[] = {
        0, 1, 4,
        1, 3, 4,
        3, 2, 4,
        2, 0, 4
    };
    
    computeNormals(vertices, 5,elements, 4);
    auto pyramid = std::make_unique<Mesh>();
    pyramid->createMesh(vertices, elements, 40, 12);
    return pyramid;
}
