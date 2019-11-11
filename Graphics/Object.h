#pragma once
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Shader.h"
#include <vector>
class Object {
public:
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
    VertexBufferObject m_VertexBufferObject;
    VertexArrayObject m_VertexArrayObject;
    ElementBufferObject m_ElementBufferObject;
    const char *m_VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
    const char *m_FragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = colour;\n"
        "}\n\0";

    Object() {}
    Object(std::vector<float> vertices, std::vector<unsigned int> indices);
    void InitialiseObjectData();
    void render(Shader &shader);
};