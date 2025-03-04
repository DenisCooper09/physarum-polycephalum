#include "PP_Shader.h"

PP_Shader PP_ShaderCreate(size_t number, ...)
{
    va_list args;
    va_start(args, number);

    GLuint program = glCreateProgram();

    for (size_t i = 0; i < number; ++i)
    {
        PP_ShaderSource ss = va_arg(args, PP_ShaderSource);

        GLuint shader = glCreateShader(ss.type);

        char *source;
        PP_ReadFile(ss.path, &source, 0);

        glShaderSource(shader, 1, (const GLchar *const *) &source, 0);
        free(source);

        glCompileShader(shader);

        int32_t compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            char message[512];
            glGetShaderInfoLog(shader, 512, 0, message);
            PP_ERROR("Shader compilation failed: %s", message);
        }

        glAttachShader(program, shader);
        glDeleteShader(shader);
    }

    va_end(args);

    glLinkProgram(program);

    int32_t linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        char message[512];
        glGetProgramInfoLog(program, 512, 0, message);
        PP_ERROR("Error linking shader program: %s", message);
    }

    return program;
}

void PP_ShaderBind(PP_Shader shader)
{
    glUseProgram(shader);
}

void PP_ShaderUnbind()
{
    glUseProgram(0);
}

void PP_ShaderDelete(PP_Shader shader)
{
    glDeleteProgram(shader);
}
