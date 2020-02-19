#include "VertexArrayObject.h"
//assignes an id to the m_ID then binds
VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
}
//binds vertex array object
void VertexArrayObject::bind() const{
    glBindVertexArray(m_ID);
}
//binds vertex array object
void VertexArrayObject::unbind() const {
    glBindVertexArray(0);
}