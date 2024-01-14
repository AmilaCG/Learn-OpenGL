#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define V_SHADER_FILE_PATH "shaders/shader.vert"
#define F_SHADER_FILE_PATH "shaders/shader.frag"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initOpengl();
void renderLoop();
void deinitOpengl();

float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

Shader* ourShader;
unsigned int vao; // Vertex array object
unsigned int vbo; // Vertex buffer object

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    initOpengl();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderLoop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deinitOpengl();
    glfwTerminate();

    return 0;
}

void initOpengl()
{
    ourShader = new Shader(V_SHADER_FILE_PATH, F_SHADER_FILE_PATH);

    glGenVertexArrays(1, &vao); // Generate vertex array object
    glGenBuffers(1, &vbo);  // Generate vertex buffer object

    // Bind vertex array object first and then bind and set vertex buffers
    glBindVertexArray(vao);

    // Bind to the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Copy vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw in wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderLoop()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader->use();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deinitOpengl()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    delete(ourShader);
    ourShader = nullptr;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
