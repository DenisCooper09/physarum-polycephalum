layout (rgba32f, binding = 0) uniform image2D u_Image;

void main()
{
    uint i = gl_GlobalInvocationID.x;

    imageStore(u_Image, ivec2(agents[i].position), vec4(1));
}
