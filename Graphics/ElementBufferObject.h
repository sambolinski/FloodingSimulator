#ifndef ELEMENTBUFFEROBJECT_H
#define ELEMENTBUFFEROBJECT_H
#include <glad/glad.h> 
#include <vector>
//Made using LearnOpenGL, used when rendering indices as well as vertices, more efficient rendering
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

