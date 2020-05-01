#include "Shader.h"

Shader::Shader() {
}
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Shaders, and https://www.youtube.com/watch?v=71BLZwRGUJE
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode (for LearnOpenGL)
//Comment: Class made with help from tutorial and the youtube tutorial, not directly taken. Code adapted

//loads the shaders used, shaders are in the object.h for that particular render object.
void Shader::loadShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Shaders, and https://www.youtube.com/watch?v=71BLZwRGUJE
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode (for LearnOpenGL)
//Comment: Class made with help from tutorial and the youtube tutorial, not directly taken. Code adapted

//same as the other load shader but also includes geometry shader
//Begin
void Shader::loadShader(const char* vertexShaderSource, const char* fragmentShaderSource, const char* geometryShaderSource) {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int geometryShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, geometryShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}
void Shader::use() {
    glUseProgram(m_ID);
}
void Shader::deleteShader() {
    glDeleteProgram(m_ID);
}
//End
//Begin -This section directly taken
void Shader::setValue(const std::string &uniformName, const int value) {
    glUniform1i(glGetUniformLocation(m_ID, uniformName.c_str()), value);
}
void Shader::setValue(const std::string &uniformName, const float value) {
    glUniform1i(glGetUniformLocation(m_ID, uniformName.c_str()), value);
}
void Shader::setValue(const std::string &uniformName, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_ID, uniformName.c_str()),1, &value[0]);
}
void Shader::setValue(const std::string &uniformName, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_ID, uniformName.c_str()), 1, &value[0]);
}
void Shader::setValue(const std::string &uniformName, const glm::vec4 &value) {
    glUniform3fv(glGetUniformLocation(m_ID, uniformName.c_str()), 1, &value[0]);
}
void Shader::setValue(const std::string &uniformName, const glm::mat2 &value) {
    glUniformMatrix2fv(glGetUniformLocation(m_ID, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::setValue(const std::string &uniformName, const glm::mat3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::setValue(const std::string &uniformName, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
}
//End