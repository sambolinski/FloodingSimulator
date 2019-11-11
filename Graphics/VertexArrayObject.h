#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H
#include <glad/glad.h> 
class VertexArrayObject {
private:
    unsigned int m_ID;
public:

    VertexArrayObject();
    void bind() const;
    void unbind() const;
};
#endif // !VERTEXARRAYOBJECT_H

