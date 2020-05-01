#include "ElementBufferObject.h"
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle and https://www.youtube.com/watch?v=oD1dvfbyf6A&t=1279s
//Author: Joey de Vries and "The Cherno"
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Used The Cherno tutorial to help with asbtraction 

//generates the buffer id and binds assignes the indices to the buffer
//Same licensing as header file, made with help form tutorial and adapted
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