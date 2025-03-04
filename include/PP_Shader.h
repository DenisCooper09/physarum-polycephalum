#pragma once

#include <string.h>
#include <stdarg.h>

#include <glad/glad.h>

#include "PP_Debug.h"
#include "PP_FileReader.h"

typedef GLuint PP_Shader;

typedef struct
{
    const char *path;
    GLenum     type;

} PP_ShaderSource;

PP_Shader PP_ShaderCreate(size_t number, ...);
PP_Shader PP_ShaderCreateMono(GLenum type, GLsizei number, ...);
void PP_ShaderBind(PP_Shader shader);
void PP_ShaderUnbind();
void PP_ShaderDelete(PP_Shader shader);
