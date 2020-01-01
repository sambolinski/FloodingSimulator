#include "Controller.h"
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include "json11.hpp"

void Simulation::World::loadWorld() {
    m_OtherRenderedObjects.emplace(m_Ocean.m_Type,m_Ocean);
    loadShip();
}
void Simulation::World::loadShip() {
    /*
    00000                   If this is the unput the bottom layer will be steel followed by a second layer above of steel,
    00S00                   0 means ignore, 
    00000
    -
    00000
    00S00
    00000
    */
    const unsigned int LAYER_HEIGHT = 50;
    const unsigned int LAYER_WEIGHT = 50;
    auto materialJson = loadMaterial();

    std::string line;
    std::ifstream shipFile;
    shipFile.open("Ships/twobythree.json");
    int xLayer = 0;
    int yLayer = 0;
    int zLayer = 0;

    glm::vec4 colour;
    float density;
    bool gravityEnabled;

    std::string buffer;
    std::string fileLine;
    while (std::getline(shipFile, fileLine)) {
        buffer += fileLine + "\n";
    }

    std::string err;
    auto json = json11::Json::parse(buffer, err);
    if (!err.empty()) {
        std::printf("Failed: %s\n", err.c_str());
    } else {
        std::printf("Result: %s\n", json.dump().c_str());
    }
    m_Ship.hullThickness = json["hullThickness"].number_value();
    m_WaterDensity = json["waterDensity"].number_value();
    std::cout << "water: " << m_WaterDensity << "\n";
    float hullDensity = materialJson[json["hullMaterial"].string_value().c_str()].number_value();
    std::cout << "HULLMATERIAL: " << json["hullMaterial"].string_value().c_str() << "HULLDENSITY: " << hullDensity << "\n";
    bool isHull;
    shipFile.close();


    shipFile.open(json["structure"].string_value().c_str());
    if (shipFile.is_open()) {
        while(getline(shipFile, line)){
            for (char& c : line) {
                if (c == '-') {
                    yLayer = -1;
                    zLayer++;
                }
                if (c!='0' && c!= ' ') {
                    if (c == 'H') {
                        colour = glm::vec4(0.310f, 0.318f, 0.329f, 1.0f);
                        gravityEnabled = true;
                        density = hullDensity;
                        isHull = true;
                    } else if (c == 'A') {
                        colour = glm::vec4(0.839f, 0.839f, 0.839f, 1.0f);
                        density = 0;
                        gravityEnabled = false;
                        isHull = false;
                    }
                    if (c != '-') {
                        Node node;
                        node.m_Object.m_Colour = colour;
                        node.m_Density = density;
                        node.m_IsHull = isHull;
                        node.m_GravityEnabled = gravityEnabled;
                        node.m_Position = glm::vec3((yLayer)*(1/node.m_Scale.x), (zLayer)*(1 / node.m_Scale.y), (xLayer)*(1 / node.m_Scale.z));
                        //node.m_Position = glm::vec3(0.0f, 100.0f, 0.0f); // DELETE AFTER TESTING
                        std::string positionalKey =  std::to_string(xLayer)+"-"
                                                    +std::to_string(yLayer)+"-"
                                                    +std::to_string(zLayer);
                        node.m_LastPosition = node.m_Position;
                        m_Ship.m_NodeList.emplace(positionalKey,node);
                    }
                }
                xLayer++;
            }
            xLayer = 0;
            yLayer++;
        }
        shipFile.close();
    } else {
        std::cout << "FILE NOT OPEN\n";
    }

    
    unsigned int TESTCOUNTER = 0;
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    for (nodeIter i = m_Ship.m_NodeList.begin(); i != m_Ship.m_NodeList.end(); i++) {
        if (i->second.m_IsHull) {
            //Neighbour counter
            //3 = corner
            //2 = edge
            //1 = cross
            int nodeOrientationType = m_Ship.getNodePosition(i->first);
            i->second.m_EmptyVolumePercentage = ((pow(nodeToNodeDistanceMetre,3)*pow(0.5,nodeOrientationType))-(pow(nodeToNodeDistanceMetre, 2)*pow(0.5,nodeOrientationType-1)*m_Ship.hullThickness*nodeOrientationType));
            i->second.m_Mass = calibrate(i->second.m_EmptyVolumePercentage * i->second.m_Density,100,nodeToNodeDistanceMetre);
            std::cout << "node: " << i->second.toString() << ", NODETYPE: " << nodeOrientationType << ",   ";
            std::cout << "asds: " << i->second.m_EmptyVolumePercentage << "\n";
            //0.93325
        }
        //springs
        xLayer = 0;
        yLayer = 0;
        zLayer = 0;
        unsigned int counter = 0;
        // turns string into it's coordinates in relation to others
        std::string xLayerString = "";
        std::string yLayerString = "";
        std::string zLayerString = "";
        //NEED TO SPLIT STRING INTO ARRAY BY -
        for (char c : i->first) {
            if (c != '-') {
                switch (counter) {
                    case 0:
                        xLayerString += c;
                        break;
                    case 1:
                        yLayerString += c;
                        break;
                    case 2:
                        zLayerString += c;
                        break;
                }
            } else {
                counter++;
            }
        }
        std::stringstream(xLayerString) >> xLayer;
        std::stringstream(yLayerString) >> yLayer;
        std::stringstream(zLayerString) >> zLayer;
        std::string currentKey = std::to_string(xLayer) + "-"
            +std::to_string(yLayer) + "-"
            +std::to_string(zLayer);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    if (x != 0 || y != 0 || z != 0) {
                        std::string keyCheck = std::to_string(xLayer + x) +"-"
                            +std::to_string(yLayer + y) +"-"
                            +std::to_string(zLayer + z);
                        std::map<std::string, PhysicsObjects::PhysicsObject>::iterator comparableNodeIter = m_Ship.m_NodeList.find(keyCheck);

                        if (comparableNodeIter != m_Ship.m_NodeList.end()) {
                            std::map<std::string, PhysicsObjects::PhysicsSpring>::iterator comparableSpringIterOne = m_Ship.m_SpringList.find(keyCheck + currentKey);
                            std::map<std::string, PhysicsObjects::PhysicsSpring>::iterator comparableSpringIterTwo = m_Ship.m_SpringList.find(currentKey + keyCheck);
                            if (comparableSpringIterOne != m_Ship.m_SpringList.end()) {
                            } else if (comparableSpringIterTwo != m_Ship.m_SpringList.end()) {
                            } else {
                                PhysicsObjects::PhysicsSpring physicsSpring;
                                physicsSpring.m_RenderObject.m_Colour = glm::vec4(0.310f, 0.318f, 0.329f, 1.0f);
                                physicsSpring.m_FirstObject = &m_Ship.m_NodeList.at(keyCheck);
                                physicsSpring.m_SecondObject = &m_Ship.m_NodeList.at(currentKey);
                                physicsSpring.m_EquilibriumDistance = abs(glm::length(physicsSpring.m_SecondObject->m_Position - physicsSpring.m_FirstObject->m_Position));
                                physicsSpring.initRenderObject();
                                m_Ship.m_SpringList.emplace(keyCheck + currentKey, physicsSpring);
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "NODE COUNT: " << m_Ship.m_NodeList.size() << "\n";
    std::cout << "SPRING COUNT: " << m_Ship.m_SpringList.size() << "\n";    
}

json11::Json Simulation::World::loadMaterial() {
    std::ifstream materialFile;
    materialFile.open("Materials/Materials.json");
    std::string buffer;
    std::string fileLine;
    while (std::getline(materialFile, fileLine)) {
        buffer += fileLine + "\n";
    }
    std::string err;
    materialFile.close();
    return json11::Json::parse(buffer, err);
}

int Simulation::Ship::getNodePosition(const std::string &objectCoord) {

    std::map<std::string, PhysicsObjects::PhysicsObject>::iterator comparableNodeIter = m_NodeList.find(objectCoord);
    int xLayer = 0;
    int yLayer = 0;
    int zLayer = 0;
    unsigned int counter = 0;
    std::string xLayerString = "";
    std::string yLayerString = "";
    std::string zLayerString = "";
    for (char c : comparableNodeIter->first) {
        if (c != '-') {
            switch (counter) {
            case 0:
            xLayerString += c;
            break;
            case 1:
            yLayerString += c;
            break;
            case 2:
            zLayerString += c;
            break;
            }
        } else {
            counter++;
        }
    }

    unsigned int neighbourCounter = 0;
    std::stringstream(xLayerString) >> xLayer;
    std::stringstream(yLayerString) >> yLayer;
    std::stringstream(zLayerString) >> zLayer;
    std::string checkX1 = std::to_string(xLayer + 1) + "-" + yLayerString + "-" + zLayerString;
    std::string checkX2 = std::to_string(xLayer - 1) + "-" + yLayerString + "-" + zLayerString;
    std::string checkY1 = xLayerString + "-" + std::to_string(yLayer + 1) + "-" + zLayerString;
    std::string checkY2 = xLayerString + "-" + std::to_string(yLayer - 1) + "-" + zLayerString;
    std::string checkZ1 = xLayerString + "-" + yLayerString + "-" + std::to_string(zLayer + 1);
    std::string checkZ2 = xLayerString + "-" + yLayerString + "-" + std::to_string(zLayer - 1);
    std::string checks[6] = { checkX1, checkY1, checkZ1, checkX2, checkY2, checkZ2};

    unsigned int xCount = 0;
    unsigned int yCount = 0;
    unsigned int zCount = 0;
    counter = 1;
    for (std::string check : checks) {
        comparableNodeIter = m_NodeList.find(check);
        if (comparableNodeIter != m_NodeList.end() && comparableNodeIter->second.m_IsHull) {
            if (counter % 3 == 1) {
                xCount++;
            } else if (counter % 3 == 2) {
                yCount++;
            } else if (counter % 3 == 0) {
                zCount++;
            }
            neighbourCounter++;
        }
        counter++;
    }
    if (xCount >= 4 || yCount >= 4 || zCount >= 4) {
        return 1;
    }else if (xCount < 2 && yCount < 2 && zCount < 2) {
        return 3;
    } else {
        return 2;
    }
    return -1;
}

Simulation::Controller::Controller(Camera &camera) {
    m_Renderer.setCamera(camera);
    m_World.loadWorld();
    m_Renderer.initialiseSceneRender();
    m_DeltaTime = 0.02f;
}

void Simulation::Controller::update() {
    float currentTime = glfwGetTime();
    m_LastTime = currentTime;
    m_Renderer.m_Camera->updateCameraSpeed(m_DeltaTime);
    updatePhysics();
    updateGraphics();
}

// Credit: Luke Wren
void Simulation::Controller::updatePhysics() {
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    typedef std::map<std::string, PhysicsObjects::PhysicsSpring>::iterator springIter;

    //GRAVITY IS CURRENTLY DISABLED IF YOU SEE NO Z MOVEMENT
    for (nodeIter i = m_World.m_Ship.m_NodeList.begin(); i != m_World.m_Ship.m_NodeList.end(); i++) {
        if (i->second.m_GravityEnabled){
            //std::cout << i->second.toString() << ",masss: " << i->second.m_Mass << "\n";
            i->second.applyForce(m_World.calibrate(m_World.m_GravityAcceleration*i->second.m_Mass,100,m_World.nodeToNodeDistanceMetre));
        }
        //Buoyancy
        if(i->second.m_Position.y <= m_World.m_Ocean.m_Position.y){
            float percentageBelowWater = 1;
            if (abs(m_World.m_Ocean.m_Position.y - i->second.m_Position.y) <= 100) {
                percentageBelowWater = abs(m_World.m_Ocean.m_Position.y - i->second.m_Position.y) / 100;
            }
            std::cout << i->second.toString() << ", percentage below: " << percentageBelowWater << "\n";
            std::cout << "buoyant: " << glm::to_string(m_World.calibrate(m_World.m_GravityAcceleration*i->second.m_EmptyVolumePercentage*m_World.m_WaterDensity, 100, m_World.nodeToNodeDistanceMetre)*percentageBelowWater*(-1.0f)) << "\n";
            i->second.applyForce(m_World.calibrate(m_World.m_GravityAcceleration*i->second.m_EmptyVolumePercentage*m_World.m_WaterDensity,100,m_World.nodeToNodeDistanceMetre)*percentageBelowWater*(-1.0f));
        
            //drag force
            // F = pv^2Ac/2
            // F = force
            // p = density of fluid
            // v = velocity of object
            // c = coefficient which will be 1
            // A = area of drag. corner edge and cross will be different
        }
        i->second.update(m_DeltaTime);
        i->second.m_Force = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    //Spring physics update
    //first for loop used as iterations. Greater number of iterations == greater accuracy but more expensive to compute  

    unsigned int counter = 0;
    for (int k = 0; k < 1; k++) {
        for (int i = 0; i < 1; i++) {
            for (springIter j = m_World.m_Ship.m_SpringList.begin(); j != m_World.m_Ship.m_SpringList.end(); j++) {
                j->second.update();
            }
        }
        for (springIter j = m_World.m_Ship.m_SpringList.begin(); j != m_World.m_Ship.m_SpringList.end(); j++) {
            float dampingAmount = pow(0.5f,m_DeltaTime);
            j->second.applyDamping(dampingAmount);
            if (k == 0) {
                j->second.updateRenderObject(false,100.0f);
            }
        }
    }
    
    /*
    Buoyancy idea. 
    Split surface nodes into groups of four
    SS
    SS
    work out gradient of face between them
    use that in figuring buoyant upward force
    */
}

glm::vec3 Simulation::Controller::applyForce() {
    return glm::vec3(0, 0, 0);
}

void Simulation::Controller::updateGraphics() {
    m_Renderer.clear();
    m_Renderer.render(m_World.m_OtherRenderedObjects);
    m_Renderer.render(m_World.m_Ship.m_NodeList);
    m_Renderer.renderSprings(m_World.m_Ship.m_SpringList);
}

glm::vec3 Simulation::World::calibrate(glm::vec3 data, const float worldDistance, const float realLifeDistance) {
    glm::vec3 scaled = data * worldDistance / realLifeDistance;
    return scaled;
}
float Simulation::World::calibrate(float data, const float worldDistance, const float realLifeDistance) {
    float scaled = data * worldDistance / realLifeDistance;
    return scaled;
}

//FOR DEBUG
glm::vec3 Simulation::Ship::averagePosition() {
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    glm::vec3 averagePosition = m_NodeList.begin()->second.m_Position;
    for (nodeIter i = m_NodeList.begin(); i != m_NodeList.end(); i++) {
        averagePosition += i->second.m_Position;
    }
    averagePosition /= m_NodeList.size();
    return averagePosition;
}