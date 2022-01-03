#version 330 core

// Input data mapped from attribute 1 of the bound VAO
layout(location = 1) in vec3 vertexPos;

void main(){
    // Global variable that must be set in the vertex shader.
    gl_Position = vec4(vertexPos, 1.0);
}