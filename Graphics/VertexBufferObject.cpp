#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(std::vector<float> &vertices) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
}
void VertexBufferObject::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void VertexBufferObject::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}