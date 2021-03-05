#include "Primitive.h"
#include <iostream>
#include "Mesh.h"
#include <glm/glm.hpp>

using namespace std;

namespace
{

// compute normals at each vertex by averaging normals of every triangle
// which the vertex belongs to, i.e. Phong normals
void computeNormals(vector<GLfloat> & vertices,
    const vector<GLuint> & elements)
{
    GLuint numElements = elements.size() / 3;
    GLuint numVertices = vertices.size() / 8;

    // compute normals for each triangle and add them to corresponding vertices
    for (GLuint i = 0; i < numElements; ++i)
    {
        // 3 vertices of a given element
        GLuint A = elements[i*3];
        GLuint B = elements[i*3+1];
        GLuint C = elements[i*3+2];
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
    std::vector<GLfloat> vertices{
        // x     y     z        u      v      nx    ny    nz
        -0.5f, 0.0f, -0.5f,   0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  10.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 10.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  10.0f, 10.0f,  0.0f, 0.0f, 0.0f
    };
    
    std::vector<GLuint> elements{
        0, 2, 1,
        1, 2, 3
    };

    computeNormals(vertices, elements);
    return std::make_unique<Mesh>(vertices, elements);
}

std::unique_ptr<Mesh> makeCube()
{
    // 3 coordinates in 3D space, 2 texture coordinates, 3 normals.
    // normals are initialized with zeros and are computed later.
    std::vector<GLfloat> vertices{
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

    std::vector<GLuint> elements{
        0, 2, 1,
        1, 2, 3,
        1, 3, 7,
        5, 1, 7,
        4, 5, 7,
        4, 7, 6,
        0, 4, 2,
        6, 2, 4
    };

    computeNormals(vertices, elements);
    return  std::make_unique<Mesh>(vertices, elements);
}

std::unique_ptr<Mesh> makePyramid()
{
    // 3 coordinates in 3D space, 2 texture coordinates, 3 normals.
    // normals are initialized with zeros and are computed later.
    std::vector<GLfloat> vertices{
        // x     y      z      u     v      nx    ny    nz
        -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f, 0.5f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    std::vector<GLuint> elements{
        0, 1, 4,
        1, 3, 4,
        3, 2, 4,
        2, 0, 4
    };
    
    computeNormals(vertices, elements);
    return std::make_unique<Mesh>(vertices, elements);
}
