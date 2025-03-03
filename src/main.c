#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PP_Debug.h"

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

    GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", 0, 0);
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

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
