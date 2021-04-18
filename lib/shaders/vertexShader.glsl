#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 posUV;
out vec3 normal;
out vec3 pos3D;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);

    posUV = tex;
    
    normal = mat3(transpose(inverse(model)))*norm;
    
    pos3D = (model * vec4(pos, 1.0)).xyz; 
}
