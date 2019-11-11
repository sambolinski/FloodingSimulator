#include "Shader.h"

Shader::Shader() {
}
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
void Shader::use() {
    glUseProgram(m_ID);
}
void Shader::deleteShader() {
    glDeleteProgram(m_ID);
}
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