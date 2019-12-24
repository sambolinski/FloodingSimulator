#include "VertexBufferObject.h"
#include <iostream>
VertexBufferObject::VertexBufferObject(std::vector<float> &vertices) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}
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