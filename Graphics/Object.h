#pragma once
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Shader.h"
#include <vector>

//Stores render data about objects.

class Object {
public:
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
    VertexBufferObject m_VertexBufferObject;
    VertexArrayObject m_VertexArrayObject;
    ElementBufferObject m_ElementBufferObject;
    unsigned int m_Mode;
    const char *m_VertexShaderSource;
    const char *m_FragmentShaderSource;
    const char *m_GeometryShaderSource;
    glm::vec4 m_Colour;

    Object() {}
    Object(std::vector<float> vertices, std::vector<unsigned int> indices);
    void InitialiseObjectData();
    void render(Shader &shader);
};