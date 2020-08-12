#include "Mesh.h"

#include <iostream>

Mesh::Mesh() :
    VAO(0),
    VBO(0),
    EBO(0),
    idxCount(0)
{ }

Mesh::~Mesh()
{
    deleteMesh();
}

void Mesh::createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices)
{
    idxCount = numIndices;

    // create a Vertex Array Object on the GPU and store its number
    glGenVertexArrays(1, &VAO);
    // activate VAO to add further objects to it
    glBindVertexArray(VAO);

    // create a Vertex Buffer Object on the GPU and store its number
    glGenBuffers(1, &VBO);
    // activate VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

    // we copy 5 values per vertex: the first 3 are the coordinates in 3D, the last 2 are the corresponding texture coordinates
    // here we tell OpenGl which values are coordinates
    // layout -> size of data (3 coordinates) -> type -> normalize -> stride -> offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
    // enable (layout = 0), no need to search to the vertices when using the vertex shader (?)
    glEnableVertexAttribArray(0);

    // here we tell OpenGL which values are texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
    // same as above
    glEnableVertexAttribArray(1);

    // create a Element Buffer Object on the GPU and store its number
    glGenBuffers(1, &EBO);
    // activate EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // copy elements' indices to the GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

    // deactivate VAO, no need to edit this object (should it be here or after VBO and EBO?)
    glBindVertexArray(0);
    // deactivate VBO, no need to edit the vertex data any more
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // deactivate EBO, no need to edit the index data any more
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void Mesh::renderMesh()
{
    // activate VAO, VAO = object (?)
    glBindVertexArray(VAO);
    // activate EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // 0 is a nullptr to smth
    glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
    // deactivate VAO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void Mesh::deleteMesh()
{
    // free GPU memory
    if (EBO != 0)
    {
        glDeleteBuffers(1,&EBO);
        EBO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1,&VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        glDeleteVertexArrays(1,&VAO);
        VAO = 0;
    }

    idxCount = 0;
}
