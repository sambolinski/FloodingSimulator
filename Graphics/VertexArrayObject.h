#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H
#include <glad/glad.h> 
//Made using LearnOpenGL
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle, https://www.youtube.com/watch?v=bTHqmzjm2UI
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Code adapted. Used youtube tutorial to help with abstraction
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

