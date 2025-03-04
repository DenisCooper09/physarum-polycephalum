#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D u_Image;

struct Agent
{
    vec2 position;
    float heading;

    float _padding1[1];
};

layout (std430, binding = 1) buffer Agents
{
    Agent agents[];
};

void main()
{
    uint i = gl_GlobalInvocationID.x;

    imageStore(u_Image, ivec2(agents[i].position), vec4(1));
}
