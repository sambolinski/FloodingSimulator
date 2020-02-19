#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H
#include <glad/glad.h> 
//Made using LearnOpenGL
//used to have multiple data about a vetex in one buffer, such as position, colour, texture coordinate
class VertexArrayObject {
private:
    unsigned int m_ID;
public:

    VertexArrayObject();
    void bind() const;
    void unbind() const;
};
#endif // !VERTEXARRAYOBJECT_H

