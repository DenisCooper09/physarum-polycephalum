#version 460 core

layout (location = 0) in vec3 i_Vertices;

void main()
{
    gl_Position = vec4(i_Vertices, 1);
}
