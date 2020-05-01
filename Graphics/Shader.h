#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Shaders, and https://www.youtube.com/watch?v=71BLZwRGUJE
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode (for LearnOpenGL)
//Comment: Class made with help from tutorial and the youtube tutorial, not directly taken. Code adapted

//made using LearnOpenGL tutorial
class Shader {
public:
    unsigned int m_ID;
    Shader();
    void loadShader(const char* vertexShaderSource, const char* fragmentShaderSource);
    void loadShader(const char* vertexShaderSource, const char* fragmentShaderSource, const char* geometryShaderSource);
    void use();
    void deleteShader();
    //functions to change the values in shader
    //This section directly taken
    //Begin
    void setValue(const std::string &uniformName, const int value);
    void setValue(const std::string &uniformName, const float value);
    void setValue(const std::string &uniformName, const glm::vec2 &value);
    void setValue(const std::string &uniformName, const glm::vec3 &value);
    void setValue(const std::string &uniformName, const glm::vec4 &value);
    void setValue(const std::string &uniformName, const glm::mat2 &value);
    void setValue(const std::string &uniformName, const glm::mat3 &value);
    void setValue(const std::string &uniformName, const glm::mat4 &value);
    //End
};
#endif