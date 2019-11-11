#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Graphics/Renderer.h"
#include "Physics/PhysicsObject.h"
#include "Physics/Node.h"
#include <vector>
#include "UserInput.h"
#include <GLFW/glfw3.h>
namespace Simulation {
    struct World {
        std::vector<PhysicsObject> m_Ship;
        void loadShip();
    };
    class Controller {
    public:
        Renderer m_Renderer;
        UserInput m_UserInput;
        World m_World;
        float m_DeltaTime = 0.0f;
        float m_LastTime = 0.0f;

        Controller(UserInput &userInput);
        void update();
        void updatePhysics();
        void updateUserInput();
        void updateGraphics();
    };
};
#endif // !CONTROLLER_H
