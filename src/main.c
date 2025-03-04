#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "PP_Debug.h"
#include "PP_Shader.h"

#define PP_TEXTURE_WIDTH 64
#define PP_TEXTURE_HEIGHT 64
#define PP_AGENTS_NUMBER 128

#define PP_PI 3.141593f
#define PP_2PI 6.283185f

typedef struct
{
    GLfloat x, y;
} PP_Vec2;

typedef struct
{
    PP_Vec2 position;
    GLfloat heading;

    GLfloat _padding1[1];
} PP_Agent;

static void GLFW_ErrorCallback(int error_code, const char *description)
{
    PP_ERROR("GLFW ERROR: 0x%08X - %s", error_code, description);
}

static void GLFW_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static float randf(float min, float max)
{
    float scale = rand() / (float) RAND_MAX; // [0, 1.0]
    return min + scale * (max - min);        // [min, max]
}

int main(void)
{
    srand(time(0));

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

    PP_Shader test_compute_shader = PP_ShaderCreate(
            1,
            (PP_ShaderSource) {"../../shaders/Agent.glsl", GL_COMPUTE_SHADER}
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

    glBindVertexArray(VAO);

    GLuint texture;
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PP_TEXTURE_WIDTH, PP_TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    PP_Agent agents[PP_AGENTS_NUMBER];

    for (size_t i = 0; i < PP_AGENTS_NUMBER; ++i)
    {
        agents[i].position = (PP_Vec2) {randf(0, PP_TEXTURE_WIDTH), randf(0, PP_TEXTURE_HEIGHT)};
        agents[i].heading  = randf(-PP_2PI, PP_2PI);
    }

    GLuint ssbo;
    glGenBuffers(1, &ssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof agents, agents, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        PP_ShaderBind(test_compute_shader);
        glDispatchCompute(PP_AGENTS_NUMBER, 1, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        PP_ShaderBind(main_shader);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    PP_ShaderDelete(main_shader);

    return 0;
}
