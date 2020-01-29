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
#include "json11.hpp"
namespace Simulation {
    struct Ship {
        std::map<std::string,PhysicsObjects::PhysicsObject> m_NodeList;
        std::map<std::string,PhysicsObjects::PhysicsSpring> m_SpringList;
        float hullThickness;
        unsigned int nodesBelowWater = 0;
        float totalFloodedVolume = 0;
        int numberOfNodesFlooded = 0;
        int getNodePosition(const std::string &objectCoord);
        std::string hullType;
        std::vector<PhysicsObjects::PhysicsObject*> m_OuterNodes;

        float m_ShipMass = 0;
        float m_TotalVolume = 0;
        float calculateList();
        float calculateTrim();
        float calculateMaximumWaterAllowed(glm::vec3 &gravity, float &waterDensity);
        void findOuterNodes();
        std::string floodingNodeID;
        glm::vec3 averagePosition();
    };
    struct World {
        const float nodeToNodeDistanceMetre = 2;
        glm::vec3 m_GravityAcceleration = glm::vec3(0.0f,-9.81f,0.0f);
        Simulation::Ship m_Ship;
        std::map<std::string, PhysicsObjects::PhysicsObject> m_OtherRenderedObjects;

        Ocean m_Ocean;
        float m_WaterDensity = 1000; //density of water in kgm^-3

        void loadWorld(std::string &fileName);
        json11::Json loadMaterial();
        void loadShip(std::string &fileName);

        glm::vec3 calibrate(glm::vec3 data, const float worldDistance, const float realLifeDistance);
        float calibrate(float data, const float worldDistance, const float realLifeDistance);
    };
    class Controller {
    public:
        Renderer m_Renderer;
        World m_World;
        float m_DeltaTime = 0.0f;
        float m_TotalTimeElapsed = 0;
        float m_LastTime = 0.0f;
        bool incrementalFlooding = false; //for testing incremental flooding only
        bool floodingStart = false;
        int currentRoundedTime = 0;
        std::map<std::string, PhysicsObjects::PhysicsObject> m_FloodingList; //list of nodes that are currently flooding

        Controller(Camera &camera, std::string &fileName);
        void update();
        glm::vec3 applyForce();

        //functions to do with flooding
        void startFlooding(PhysicsObjects::PhysicsObject &firstNode);
        float calculateFloodingAmount(float deltaTime);
        void addAdjacentNodes(std::map<std::string, PhysicsObjects::PhysicsObject>::iterator node);

        //update function
        void updatePhysics();
        void updateGraphics();

        //data handling
        std::string m_FileName;
        void placeStartingData();
        void placeIncrementalData();
    };
};
#endif // !CONTROLLER_H
