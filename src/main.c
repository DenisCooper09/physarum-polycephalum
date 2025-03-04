#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PP_Debug.h"
#include "PP_Shader.h"

static void GLFW_ErrorCallback(int error_code, const char *description)
{
    PP_ERROR("GLFW ERROR: 0x%08X - %s", error_code, description);
}

static void GLFW_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwSetErrorCallback(GLFW_ErrorCallback);

    if (!glfwInit())
    {
        PP_FATAL(-1, "GLFW failed to initialize.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "P. Polycephalum Simulation", 0, 0);
    if (!window)
    {
        glfwTerminate();
        PP_FATAL(-2, "GLFW failed to create window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GLFW_FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        PP_FATAL(-3, "GLAD failed to load OpenGL loader.");
    }

    PP_Shader main_shader = PP_ShaderCreate(
            2,
            (PP_ShaderSource) {"../../shaders/Vertex.glsl", GL_VERTEX_SHADER},
            (PP_ShaderSource) {"../../shaders/Fragment.glsl", GL_FRAGMENT_SHADER}
    );

    GLuint VAO, VBO, EBO;

    const GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    const GLubyte indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof vertices, vertices, GL_STATIC_DRAW);

    glCreateBuffers(1, &EBO);
    glNamedBufferData(EBO, sizeof indices, indices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));

    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glVertexArrayAttribBinding(VAO, 1, 0);

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayElementBuffer(VAO, EBO);

    PP_ShaderBind(main_shader);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    PP_ShaderDelete(main_shader);

    return 0;
}
