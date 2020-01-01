#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "../Graphics/Object.h"
#include <string>
namespace PhysicsObjects {
    class PhysicsObject {
    public:
        std::string m_Type;
        Object m_Object;
        glm::vec3 m_Position;
        glm::vec3 m_LastPosition;
        glm::vec3 m_Velocity;
        glm::vec3 m_Acceleration;
        glm::vec3 m_Scale;
        glm::vec3 m_Force;
        float m_Mass;
        bool m_GravityEnabled = false;
        bool m_UnderWater = false;
        bool m_IsHull = false;
        float m_Density;
        float m_EmptyVolumePercentage = 100;
        PhysicsObject() {
            m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
            m_Velocity = glm::vec3(0, 0, 0);
            m_Acceleration = glm::vec3(0, 0, 0);
        }
        void update(float deltaTime);
        void applyForce(glm::vec3 force);
        std::string toString() {
            return "(" + std::to_string(m_Position.x) + ", "
                       + std::to_string(m_Position.y) + ", "
                       + std::to_string(m_Position.z) +
                   ")";
        }
    };
    struct PhysicsSpring {
    public:
        float m_EquilibriumDistance;
        float m_Constant = 2.0f;
        Object m_RenderObject;
        PhysicsObjects::PhysicsObject *m_FirstObject;
        PhysicsObjects::PhysicsObject *m_SecondObject;
        void initRenderObject();
        void updateRenderObject(bool cross, float averageDistance);
        void update();
        void applyDamping(float &dampingAmount);

    };
};
#endif // !PHYSICSOBJECT_H

