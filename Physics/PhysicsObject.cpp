#include "PhysicsObject.h"
#include <iostream>

//#CONTRIBUTED CODE
//url: https://github.com/Wren6991/Ship-Sandbox/blob/master/phys.cpp
//Author: Luke Wren
//Licensing: 
//Comment: Minor adaptions to work to work with GLM and my classes
//Begin
void PhysicsObjects::PhysicsSpring::update() {
    glm::vec3 delta = m_SecondObject->m_Position - m_FirstObject->m_Position;
    float deltaLength = glm::length(delta);
    delta *= (m_EquilibriumDistance - deltaLength) / m_EquilibriumDistance * 0.85;
    m_FirstObject->m_Position -= delta * 0.5f;
    m_SecondObject->m_Position += delta * 0.5f;
}
//End

//#CONTRIBUTED CODE
//url: https://github.com/Wren6991/Ship-Sandbox/blob/master/phys.cpp
//Author: Luke Wren
//Licensing: 
//Comment: Minor adaptions to work with GLM and my classes
//Begin
void PhysicsObjects::PhysicsSpring::applyDamping(float &dampingAmount) {
    glm::vec3 springDifference = glm::normalize(m_FirstObject->m_Position - m_SecondObject->m_Position);
    springDifference *= glm::dot((m_FirstObject->m_Position - m_FirstObject->m_LastPosition - (m_SecondObject->m_Position - m_SecondObject->m_LastPosition)), springDifference)*dampingAmount;
    m_FirstObject->m_LastPosition += springDifference;
    m_SecondObject->m_LastPosition -= springDifference;
}
//End
void PhysicsObjects::PhysicsSpring::initRenderObject() {
    m_RenderObject.m_Mode = 2;
    m_RenderObject.m_Vertices = {
        0.0f, 0.0f , 0.0f,
        0.0f, 100.0f , 0.0f
    };

    m_RenderObject.m_Indices = {
        0,1
    };

    //#CONTRIBUTED CODE
    //url: https://learnopengl.com/Getting-started/Shaders
    //Author: Joey de Vries
    //Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
    //Comment: taken directly from the tutorial
    //Begin
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
    //End
    m_RenderObject.InitialiseObjectData();
}
//updates the spring graphics made on the two connected nodes
//determines whether diagonal springs are drawn
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
//floods the node
//if the amount is greater than the maximum allowed per node no more will be added.
//an amount is returned depending on whether the node is at the max amount or not. This is used for calculating the current ship flooded volume
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
//#CONTRIBUTED CODE
//url: https://github.com/Wren6991/Ship-Sandbox/blob/master/phys.cpp
//Author: Luke Wren
//Licensing: 
//Comment: Minor adaptions to work with my classes, and to work with my calibration settings
//Begin
void PhysicsObjects::PhysicsObject::update(float deltaTime) {
    glm::vec3 lastPosition = m_Position;
    float calibration = 100 / 2;
    m_Position += (m_Position - m_LastPosition) + (m_Force * calibration) * (deltaTime * deltaTime/ m_Mass);
    m_Velocity = ((m_Position - m_LastPosition) / calibration) / deltaTime;
    m_LastPosition = lastPosition;
}
//End