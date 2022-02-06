#version 330 core

// Input vector with 3D coordinates of a vertex.
layout(location = 0) in vec3 vertexCoordinates;
// Output vector that will be passed to the fragment shader.
out vec3 position3D;

void main() {
    // Built-in vector of homogeneous vertex coordinates.
    // Has to be assigned before rasterization.
    gl_Position = vec4(vertexCoordinates, 1.0);
    // Pass the coordinates to the fragment shader without change.
    position3D = vertexCoordinates;
}