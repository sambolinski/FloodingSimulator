#include "PhysicsObject.h"
#include <iostream>

//Credit: Luke Wren
void PhysicsObjects::PhysicsSpring::update() {
    glm::vec3 delta = m_SecondObject->m_Position - m_FirstObject->m_Position;
    float deltaLength = glm::length(delta);
    delta *= (m_EquilibriumDistance - deltaLength) / m_EquilibriumDistance * 0.85;
    //std::cout << "deltaLength: " << deltaLength << ", s_EquilibriumDistance: " << PhysicsObjects::PhysicsSpring::s_EquilibriumDistance << "\n";
    m_FirstObject->m_Position -= delta * 0.5f;
    m_SecondObject->m_Position += delta * 0.5f;
    /*originally

    m_FirstObject->m_Position -= delta * m_Second->m_Mass;
    m_SecondObject->m_Position += delta * m_First->m_Mass;
    */
}

//Credit: Luke Wren
void PhysicsObjects::PhysicsSpring::applyDamping(float &dampingAmount) {
    glm::vec3 springDifference = glm::normalize(m_FirstObject->m_Position - m_SecondObject->m_Position);
    springDifference *= glm::dot((m_FirstObject->m_Position - m_FirstObject->m_LastPosition - (m_SecondObject->m_Position - m_SecondObject->m_LastPosition)), springDifference)*dampingAmount;
    m_FirstObject->m_LastPosition += springDifference;
    m_SecondObject->m_LastPosition -= springDifference;
}

void PhysicsObjects::PhysicsSpring::initRenderObject() {
    m_RenderObject.m_Mode = 2;
    m_RenderObject.m_Vertices = {
        0.0f, 0.0f , 0.0f,
        0.0f, 100.0f , 0.0f
    };

    m_RenderObject.m_Indices = {
        0,1
    };
    m_RenderObject.m_VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";

    m_RenderObject.m_GeometryShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void generateLine(int index);\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
    m_RenderObject.m_FragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = colour;\n"
        "}\n\0";
    m_RenderObject.InitialiseObjectData();
}
void PhysicsObjects::PhysicsSpring::updateRenderObject(bool cross, float averageDistance) {
    if (cross == true) {
        m_RenderObject.m_Vertices[0] = (m_SecondObject->m_Position.x - m_FirstObject->m_Position.x);
        m_RenderObject.m_Vertices[1] = (m_SecondObject->m_Position.y - m_FirstObject->m_Position.y);
        m_RenderObject.m_Vertices[2] = (m_SecondObject->m_Position.z - m_FirstObject->m_Position.z);
        m_RenderObject.m_Vertices[3] = 0;
        m_RenderObject.m_Vertices[4] = 0;
        m_RenderObject.m_Vertices[5] = 0;

        m_RenderObject.m_VertexBufferObject.update(m_RenderObject.m_Vertices);
    } else {
        if (abs(glm::length(m_SecondObject->m_Position - m_FirstObject->m_Position)) <= (averageDistance * 1.1f)) {
            m_RenderObject.m_Vertices[0] = (m_SecondObject->m_Position.x - m_FirstObject->m_Position.x);
            m_RenderObject.m_Vertices[1] = (m_SecondObject->m_Position.y - m_FirstObject->m_Position.y);
            m_RenderObject.m_Vertices[2] = (m_SecondObject->m_Position.z - m_FirstObject->m_Position.z);
            m_RenderObject.m_Vertices[3] = 0;
            m_RenderObject.m_Vertices[4] = 0;
            m_RenderObject.m_Vertices[5] = 0;
            m_RenderObject.m_VertexBufferObject.update(m_RenderObject.m_Vertices);
        } else {

            m_RenderObject.m_Vertices[0] = 0;
            m_RenderObject.m_Vertices[1] = 0;
            m_RenderObject.m_Vertices[2] = 0;
            m_RenderObject.m_Vertices[3] = 0;
            m_RenderObject.m_Vertices[4] = 0;
            m_RenderObject.m_Vertices[5] = 0;
            m_RenderObject.m_VertexBufferObject.update(m_RenderObject.m_Vertices);
        }
    }
}
void PhysicsObjects::PhysicsObject::applyForce(glm::vec3 force) {
    m_Force += force;
}

float  PhysicsObjects::PhysicsObject::flood(float amount) {
    if (m_CurrentFloodedVolume + amount < m_MaxFloodableVolume) {
        m_CurrentFloodedVolume = m_CurrentFloodedVolume + amount;
        return amount;
    } else {
        float toReturn = m_MaxFloodableVolume - m_CurrentFloodedVolume;
        m_CurrentFloodedVolume = m_MaxFloodableVolume;
        return toReturn;
    }
}
//Credit: Luke Wren
void PhysicsObjects::PhysicsObject::update(float deltaTime) {
    glm::vec3 lastPosition = m_Position;
    float calibration = 100 / 2;
    m_Position += (m_Position - m_LastPosition) + (m_Force * calibration) * (deltaTime * deltaTime/ m_Mass);
    m_Velocity = ((m_Position - m_LastPosition) / calibration) / deltaTime;
    m_LastPosition = lastPosition;
}