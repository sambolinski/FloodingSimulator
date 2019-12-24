#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
class Shader {
public:
    unsigned int m_ID;
    Shader();
    void loadShader(const char* vertexShaderSource, const char* fragmentShaderSource);
    void loadShader(const char* vertexShaderSource, const char* fragmentShaderSource, const char* geometryShaderSource);
    void use();
    void deleteShader();
    void setValue(const std::string &uniformName, const int value);
    void setValue(const std::string &uniformName, const float value);
    void setValue(const std::string &uniformName, const glm::vec2 &value);
    void setValue(const std::string &uniformName, const glm::vec3 &value);
    void setValue(const std::string &uniformName, const glm::vec4 &value);
    void setValue(const std::string &uniformName, const glm::mat2 &value);
    void setValue(const std::string &uniformName, const glm::mat3 &value);
    void setValue(const std::string &uniformName, const glm::mat4 &value);
};
#endif