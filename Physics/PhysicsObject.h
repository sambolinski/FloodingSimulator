#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "../Graphics/Object.h"
#include <string>

class PhysicsObject {
public:
    std::string m_Type;
    Object m_Object;
    glm::vec3 m_Position;
    glm::vec3 m_Velocity; 
    glm::vec3 m_Acceleration; 
    glm::vec3 m_Scale;
    bool m_PhysicsEnabled = true;
    PhysicsObject() {
        m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Velocity = glm::vec3(0, 0, 0);
        m_Acceleration = glm::vec3(0, 0, 0);
    }
};
#endif // !PHYSICSOBJECT_H

