#version 330 core

// Interpolated position passed from the vertex shader.
in vec3 position3D;
// Output fragment color.
out vec3 color;

void main()
{ 
    color = (position3D + vec3(1.,1.,2.))/2.;
}