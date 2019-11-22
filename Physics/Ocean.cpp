#include "Ocean.h"
Ocean::Ocean() {
    m_PhysicsEnabled = false;
    m_Type = "static_Ocean";
    m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_Object.m_Colour = glm::vec4(0.157f, 0.286f, 0.510f, 1.0f);
    m_Object.m_Vertices = {
        -10.0f, 0.0f, -10.0f,
         10.0f, 0.0f, -10.0f,
        -10.0f, 0.0f,  10.0f,
         10.0f, 0.0f,  10.0f
    };
    m_Object.m_Indices = {
        0, 1, 2,
        1, 2, 3
    };
    m_Object.m_VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
    m_Object.m_FragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = colour;\n"
        "}\n\0";
    m_Object.InitialiseObjectData(); 
}