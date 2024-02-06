#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define V_CONTAINER_SHADER_PATH "shaders/shader.vert"
#define F_CONTAINER_SHADER_PATH "shaders/shader.frag"

#define V_LIGHT_SHADER_PATH "shaders/shader_light.vert"
#define F_LIGHT_SHADER_PATH "shaders/shader_light.frag"

#define MOUSE_SENSITIVITY 0.1f

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);
void initOpengl();
void renderLoop();
glm::mat4 myLookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 worldUp);
glm::vec3 getCameraDirection(const float yaw, const float pitch);
void cameraSetup(glm::vec3 position, glm::vec3* direction, glm::vec3* right, glm::vec3* up);
glm::mat4 getModelMatrix();
glm::mat4 getModelMatrix(glm::vec3 position, float rotationDeg, glm::vec3 rotationAxis);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 transform();
void deinitOpengl();

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
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

Shader* containerShader;
Shader* lightShader;
unsigned int vao; // Vertex array object
unsigned int lightVao;
unsigned int vbo; // Vertex buffer object

unsigned int texture; // Container texture object
unsigned int texture2; // awesomeface texture

const glm::vec3 world_front(0.0f, 0.0f, -1.0f);
const glm::vec3 world_up(0.0f, 1.0f, 0.0f);

glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = world_front;
glm::vec3 cameraUp = world_up;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float fov = 45.0f;

float yaw = -90.0f; // Rotation around Y axis
float pitch = 0.0f; // Rotation around X axis

bool isFirstMouse = true;
float lastMouseX = WINDOW_WIDTH / 2;
float lastMouseY = WINDOW_HEIGHT / 2;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetScrollCallback(window, scrollCallback);

    initOpengl();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

    containerShader = new Shader(V_CONTAINER_SHADER_PATH, F_CONTAINER_SHADER_PATH);
    lightShader = new Shader(V_LIGHT_SHADER_PATH, F_LIGHT_SHADER_PATH);

    glGenVertexArrays(1, &vao); // Generate vertex array object
    glGenVertexArrays(1, &lightVao);
    glGenBuffers(1, &vbo);  // Generate vertex buffer object

    // Bind vertex array object first and then bind and set vertex buffers
    glBindVertexArray(vao);

    // Bind to the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Copy vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw in wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderLoop()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    containerShader->use();
    containerShader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    containerShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(vao);

    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, world_up);
    containerShader->setMat4("view", view);

    glm::mat4 projection = getProjectionMatrix();
    containerShader->setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, world_up);
    containerShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader->use();
    lightShader->setMat4("view", view);
    lightShader->setMat4("projection", projection);

    auto lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    model = glm::mat4(1.0f);
    model = getModelMatrix(lightPos, 0.0f, world_up);
    lightShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::mat4 myLookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 worldUp)
{
    glm::vec3 cameraForward = glm::normalize(cameraPos - target);
    glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraForward));
    glm::vec3 cameraUp = glm::cross(cameraForward, cameraRight);

    glm::mat4 rotation(1.0f);
    rotation[0] = glm::vec4(cameraRight.x, cameraUp.x, cameraForward.x, 0.0f); // 0th column
    rotation[1] = glm::vec4(cameraRight.y, cameraUp.y, cameraForward.y, 0.0f); // 1st column
    rotation[2] = glm::vec4(cameraRight.z, cameraUp.z, cameraForward.z, 0.0f); // 2nd column

    glm::mat4 translation(1.0f);
    translation[3][0] = -cameraPos.x; // 3rd column, 0th row
    translation[3][1] = -cameraPos.y;
    translation[3][2] = -cameraPos.z;

    return rotation * translation;
}

glm::vec3 getCameraDirection(const float yaw, const float pitch)
{
    glm::vec3 cameraDirection;
    cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection.y = sin(glm::radians(pitch));
    cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    return cameraDirection;
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
        glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

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
    delete(containerShader);
    containerShader = nullptr;
    delete(lightShader);
    lightShader = nullptr;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    const float camera_speed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += camera_speed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= camera_speed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
    }
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (isFirstMouse)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        isFirstMouse = false;
    }

    float xOffset = xPos - lastMouseX;
    float yOffset = lastMouseY - yPos;
    lastMouseX = xPos;
    lastMouseY = yPos;

    xOffset *= MOUSE_SENSITIVITY;
    yOffset *= MOUSE_SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) { pitch = 89.0f; }
    if (pitch < -89.0f) { pitch = -89.0f; }

    glm::vec3 cameraDirection = getCameraDirection(yaw, pitch);
    cameraFront = glm::normalize(cameraDirection);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    fov -= (float)yOffset;
    if (fov < 1.0f) { fov = 1.0f; }
    if (fov > 45.0f) { fov = 45.0f; }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
