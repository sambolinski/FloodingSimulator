#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H
#include <glad/glad.h> 
#include <vector>

//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle, https://www.youtube.com/watch?v=bTHqmzjm2UI
//Author: Joey de Vries, The Cherno
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Code adapted. Used youtube tutorial to help with abstraction

//used to have multiple data about a vetex in one buffer, such as position, colour, texture coordinate
//buffer which contains data about the vertices
class VertexBufferObject {
private:
    unsigned int m_ID;
public:

    VertexBufferObject() {}
    VertexBufferObject(std::vector<float> &vertices);
    void update(std::vector<float> &vertices);
    void bind() const;
    void unbind() const;
};
#endif // !VERTEXBUFFEROBJECT_H