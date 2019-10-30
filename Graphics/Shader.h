#pragma once
#include <glad/glad.h> 
class Shader {
public:
    unsigned int shaderProgram;
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    void use();
    void deleteShader();
};