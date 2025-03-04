#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D u_Image;

void main()
{
    vec4 value = vec4(0, 0, 0, 1);
    ivec2 texel = ivec2(gl_GlobalInvocationID.xy);

    value.x = float(texel.x) / (gl_NumWorkGroups.x);
    value.y = float(texel.y) / (gl_NumWorkGroups.y);

    imageStore(u_Image, texel, value);
}
