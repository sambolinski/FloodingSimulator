#include "VertexArrayObject.h"
VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
}

void VertexArrayObject::bind() const{
    glBindVertexArray(m_ID);
}
void VertexArrayObject::unbind() const {
    glBindVertexArray(0);
}