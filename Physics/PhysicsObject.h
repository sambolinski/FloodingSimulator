#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "../Graphics/Object.h"
#include <string>

class PhysicsObject {
public:
    //static unsigned int s_ID;
    std::string m_Type;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
    Object m_Object;
    glm::vec3 m_Position;
    PhysicsObject() {
        //s_ID++;
    }
};
//unsigned int PhysicsObject::s_ID = 0;
#endif // !PHYSICSOBJECT_H

