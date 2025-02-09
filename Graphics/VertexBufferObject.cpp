#include "VertexBufferObject.h"
#include <iostream>
//Made using LearnOpenGL
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle, https://www.youtube.com/watch?v=bTHqmzjm2UI
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Code adapted. Used youtube tutorial to help with abstraction


//used to have multiple data about a vetex in one buffer, such as position, colour, texture coordinate
//generates the buffer, adds teh vertices and binds the buffer
VertexBufferObject::VertexBufferObject(std::vector<float> &vertices) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}
//This update methods updates the buffer to change the vertices of the springs
//made using https://stackoverflow.com/questions/15821969/what-is-the-proper-way-to-modify-opengl-vertex-buffer
void VertexBufferObject::update(std::vector<float> &vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &vertices[0], vertices.size()*sizeof(float));
    glUnmapBuffer(GL_ARRAY_BUFFER);
}
void VertexBufferObject::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void VertexBufferObject::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}