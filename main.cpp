#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define V_SHADER_FILE_PATH "shaders/transform_shader.vert"
#define F_SHADER_FILE_PATH "shaders/transform_shader.frag"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initOpengl();
void renderLoop();
void cameraSetup(glm::vec3 position, glm::vec3* direction, glm::vec3* right, glm::vec3* up);
glm::mat4 getModelMatrix();
glm::mat4 getModelMatrix(glm::vec3 position, float rotationDeg, glm::vec3 rotationAxis);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 transform();
void deinitOpengl();

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

Shader* ourShader;
unsigned int vao; // Vertex array object
unsigned int vbo; // Vertex buffer object
unsigned int ebo; // Element buffer object

unsigned int texture; // Container texture object
unsigned int texture2; // awesomeface texture

glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDirection;
glm::vec3 cameraRight;
glm::vec3 cameraUp;

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
    glEnable(GL_DEPTH_TEST);

    ourShader = new Shader(V_SHADER_FILE_PATH, F_SHADER_FILE_PATH);

    glGenVertexArrays(1, &vao); // Generate vertex array object
    glGenBuffers(1, &vbo);  // Generate vertex buffer object
    glGenBuffers(1, &ebo);
    glGenTextures(1, &texture);
    glGenTextures(1, &texture2);

    // Bind vertex array object first and then bind and set vertex buffers
    glBindVertexArray(vao);

    // Bind to the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Copy vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Setting up the container texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imgWidth, imgHeight, nrChannels;
    unsigned char* imgData = stbi_load("textures/container.jpg", &imgWidth, &imgHeight, &nrChannels, 0);
    if (!imgData)
    {
        std::cout << "Failed to load texture image" << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);
    
    // Setting up the awesomeface texture
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load("textures/awesomeface.png", &imgWidth, &imgHeight, &nrChannels, 0);
    if (!imgData)
    {
        std::cout << "Failed to load texture image" << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);

    // Draw in wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderLoop()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader->use();
    ourShader->setInt("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    const glm::vec3 world_up_vector(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
    glm::vec3 targetPosition(0.0f, 0.0f, 0.0f);

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    cameraPosition = glm::vec3(camX, 0.0, camZ);

    glm::mat4 view = glm::lookAt(cameraPosition, targetPosition, world_up_vector);
    ourShader->setMat4("view", view);

    glm::mat4 projection = getProjectionMatrix();
    ourShader->setMat4("projection", projection);

    for (int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = getModelMatrix(cubePositions[i], 20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void cameraSetup(glm::vec3 position, glm::vec3* direction, glm::vec3* right, glm::vec3* up)
{
    // Camera forward direction (Note that this actually points the opposite 
    // direction of what it's targeting)
    const glm::vec3 origin(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraTarget(origin);
    *direction = glm::normalize(position - cameraTarget);

    // Camera right direction
    const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    *right = glm::normalize(glm::cross(worldUp, *direction));

    // Camera up direction
    *up = glm::cross(*direction, *right);
}

glm::mat4 getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    return model;
}

glm::mat4 getModelMatrix(glm::vec3 position, float rotationDeg, glm::vec3 rotationAxis)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationDeg), rotationAxis);

    return model;
}

glm::mat4 getViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);
    // Note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    return view;
}

glm::mat4 getProjectionMatrix()
{
    glm::mat4 projection;
    projection = 
        glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    return projection;
}

glm::mat4 transform()
{
    glm::mat4 trans = glm::mat4(1.0f);
    // First rotating around (0,0,0) and THEN translating/moving to (0.5,-0.5,0). We are writing
    // this in the reverse order as below.
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    return trans;
}

void deinitOpengl()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
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
