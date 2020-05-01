#ifndef ELEMENTBUFFEROBJECT_H
#define ELEMENTBUFFEROBJECT_H
#include <glad/glad.h> 
#include <vector>
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Triangle and https://www.youtube.com/watch?v=oD1dvfbyf6A&t=1279s
//Author: Joey de Vries and "The Cherno"
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Class made with help from tutorial, not directly taken. Used The Cherno tutorial to help with asbtraction 

//generates the buffer id and binds assignes the indices to the buffer
//Same licensing as header file, made with help form tutorial and adapted

// used when rendering indices as well as vertices, more efficient rendering
class ElementBufferObject {
private:
    unsigned int m_ID;
public:
    ElementBufferObject() {}
    ElementBufferObject(std::vector<unsigned int> &indices);
    void bind() const;
    void unbind() const;
};
#endif// !ELEMENTBUFFEROBJECT_H

