#pragma once

#include <glad/glad.h>
#include <string>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    /// <summary>
    /// Use/Activate the shader program
    /// </summary>
    void use();

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void compileAndLink(const char* vShaderCode, const char* fShaderCode);
    void checkCompileErrors(unsigned int shader, std::string type);

private:
    unsigned int programID;
};
