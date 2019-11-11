#include "ElementBufferObject.h"
ElementBufferObject::ElementBufferObject(std::vector<unsigned int> &indices) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}
void ElementBufferObject::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}
void ElementBufferObject::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    glDeleteBuffers(1, &m_ID);
}