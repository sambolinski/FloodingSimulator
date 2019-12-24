#include "Object.h"
Object::Object(std::vector<float> vertices, std::vector<unsigned int> indices) {
    m_Vertices = vertices;
    m_Indices = indices;
    InitialiseObjectData();
}
void Object::InitialiseObjectData() {
    m_VertexBufferObject = VertexBufferObject(m_Vertices);
    m_VertexArrayObject = VertexArrayObject();
    m_ElementBufferObject = ElementBufferObject(m_Indices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
void Object::render(Shader &shader) {
    m_VertexBufferObject.bind();
    m_VertexArrayObject.bind(); 
    m_ElementBufferObject.bind();
    if (m_Mode == 2) {
        glDrawElements(GL_LINES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    } else if (m_Mode == 3) {
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }
    m_VertexBufferObject.unbind();
    m_VertexArrayObject.unbind();
    m_ElementBufferObject.unbind();
}