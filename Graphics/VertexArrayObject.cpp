#include "VertexArrayObject.h"
//Made using LearnOpenGL
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle, https://www.youtube.com/watch?v=bTHqmzjm2UI
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Code adapted. Used youtube tutorial to help with abstraction
//used to have multiple data about a vetex in one buffer, such as position, colour, texture coordinate

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