#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Graphics/Renderer.h"
#include "Physics/PhysicsObject.h"
#include "Physics/Ocean.h"
#include "Physics/Node.h"
#include <vector>
#include <map>
#include <string>
#include "UserInput.h"
namespace Simulation {
    struct Ship {
        std::map<std::string,PhysicsObjects::PhysicsObject> m_NodeList;
        std::map<std::string,PhysicsObjects::PhysicsSpring> m_SpringList;
        unsigned int nodesBelowWater = 0;
        glm::vec3 averagePosition();
    };
    struct World {
        const float nodeToNodeDistanceMetre = 2;


        glm::vec3 m_GravityAcceleration = glm::vec3(0.0f,-9.81f,0.0f);
        Simulation::Ship m_Ship;
        std::map<std::string, PhysicsObjects::PhysicsObject> m_OtherRenderedObjects;
        Ocean m_Ocean;
        float m_WorldForceMultiplier = 1; //used to scale the forces on every object
        void loadWorld();
        void loadShip();
    };
    class Controller {
    public:
        Renderer m_Renderer;
        World m_World;
        float m_DeltaTime = 0.0f;
        float m_LastTime = 0.0f;

        Controller(Camera &camera);
        void update();
        glm::vec3 applyForce();
        void updatePhysics();
        void updateGraphics();


        glm::vec3 calibrate(glm::vec3 data, const float worldDistance, const float realLifeDistance);
        float calibrate(float data, const float worldDistance, const float realLifeDistance);
    };
};

#endif // !CONTROLLER_H
