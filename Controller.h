#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Graphics/Renderer.h"
#include "Physics/PhysicsObject.h"
#include "Physics/Ocean.h"
#include "Physics/Node.h"
#include <vector>
#include "UserInput.h"
namespace Simulation {
    struct World {
        glm::vec3 m_GravityAcceleration = glm::vec3(0.0,-1,0.0);
        std::vector<PhysicsObject> m_Ship;
        std::vector<PhysicsObject> m_OtherRenderedObjects;
        Ocean m_Ocean;
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
    };
};
#endif // !CONTROLLER_H
