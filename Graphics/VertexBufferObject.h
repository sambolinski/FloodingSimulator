#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H
#include <glad/glad.h> 
#include <vector>
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