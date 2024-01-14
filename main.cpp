#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void init_opengl();
void render_loop();
void deinit_opengl();
void compile_and_link_shaders();

float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

// Passing color values from vertex attributes
const char* vertexShaderSource =
    "#version 330 core                                  \n"
    "layout (location = 0) in vec3 aPos;                \n"
    "layout (location = 1) in vec3 aColor;              \n"
    "out vec3 ourColor;                                 \n"
    "void main()                                        \n"
    "{                                                  \n"
    "   gl_Position = vec4(aPos, 1.0);                  \n"
    "   ourColor = aColor;                              \n"
    "}                                                  \0";

const char* fragmentShaderSource =
    "#version 330 core                                  \n"
    "out vec4 FragColor;                                \n"
    "in vec3 ourColor;                                  \n"
    "void main()                                        \n"
    "{                                                  \n"
    "   FragColor = vec4(ourColor, 1.0);                \n"
    "}                                                  \0";

// Passing a value from vertex shader to fragment shader
//const char* vertexShaderSource = 
//    "#version 330 core                                  \n"
//    "layout (location = 0) in vec3 aPos;                \n"
//    "out vec4 vertexColor;                              \n"
//    "void main()                                        \n"
//    "{                                                  \n"
//    "   gl_Position = vec4(aPos, 1.0);                  \n"
//    "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);         \n"
//    "}                                                  \0";
//
//const char* fragmentShaderSource =
//    "#version 330 core                                  \n"
//    "out vec4 FragColor;                                \n"
//    "in vec4 vertexColor;                               \n"
//    "void main()                                        \n"
//    "{                                                  \n"
//    "   FragColor = vertexColor;                        \n"
//    "}                                                  \0";

// Passing color value from the code
//const char* vertexShaderSource =
//"#version 330 core                                  \n"
//"layout (location = 0) in vec3 aPos;                \n"
//"out vec4 vertexColor;                              \n"
//"void main()                                        \n"
//"{                                                  \n"
//"   gl_Position = vec4(aPos, 1.0);                  \n"
//"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);         \n"
//"}                                                  \0";
//
//const char* fragmentShaderSource =
//"#version 330 core                                  \n"
//"out vec4 FragColor;                                \n"
//"uniform vec4 outColor;                             \n"
//"void main()                                        \n"
//"{                                                  \n"
//"   FragColor = outColor;                           \n"
//"}                                                  \0";

unsigned int shaderProgram; // Shader program object
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    init_opengl();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        render_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deinit_opengl();
    glfwTerminate();

    return 0;
}

void init_opengl()
{
    compile_and_link_shaders();

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

void render_loop()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //float timeValue = glfwGetTime();
    //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
    glUseProgram(shaderProgram);
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deinit_opengl()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
}

void compile_and_link_shaders()
{
    // Vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Set the source and compile the vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check compilation status
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Set source and compile the fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();

    // Attach vertex and fragment shaders to the program and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // We no longer will need shader objects once they got linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
