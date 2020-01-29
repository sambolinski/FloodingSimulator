#include "Controller.h"
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include <math.h> 
#include "json11.hpp"

void Simulation::World::loadWorld(std::string &fileName) {
    m_OtherRenderedObjects.emplace(m_Ocean.m_Type, m_Ocean);
    loadShip(fileName);
}
void Simulation::World::loadShip(std::string &fileName) {
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

    shipFile.open("Ships/"+ fileName+".json");
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
    m_Ship.floodingNodeID = json["floodingNode"].string_value().c_str();

    bool isHull;
    bool isBulkhead;
    shipFile.close();


    shipFile.open(json["structure"].string_value().c_str());
    if (shipFile.is_open()) {
        while (getline(shipFile, line)) {
            for (char& c : line) {
                if (c == '-') {
                    yLayer = -1;
                    zLayer++;
                }
                if (c != '0' && c != ' ') {
                    if (c == 'H') {
                        colour = glm::vec4(0.310f, 0.318f, 0.329f, 1.0f);
                        gravityEnabled = true;
                        density = hullDensity;
                        isHull = true;
                        isBulkhead = false;
                    } else if (c == 'A') {
                        colour = glm::vec4(0.839f, 0.839f, 0.839f, 1.0f);
                        density = hullDensity;
                        gravityEnabled = false;
                        isHull = false;
                        isBulkhead = false;
                    } else if (c == 'B'){
                        colour = glm::vec4(0.310f, 0.318f, 0.329f, 1.0f);
                        gravityEnabled = true;
                        density = hullDensity;
                        isHull = true;
                        isBulkhead = true;
                    }
                    if (c != '-') {
                        Node node;
                        node.m_Object.m_Colour = colour;
                        node.m_Density = density;
                        node.m_IsHull = isHull;
                        node.m_IsBulkhead = isBulkhead;
                        node.m_GravityEnabled = gravityEnabled;
                        node.m_Position = glm::vec3((yLayer)*(1 / node.m_Scale.x), (zLayer)*(1 / node.m_Scale.y), (xLayer)*(1 / node.m_Scale.z));
                        std::string positionalKey = std::to_string(xLayer) + "-"
                            + std::to_string(yLayer) + "-"
                            + std::to_string(zLayer);
                        node.m_LastPosition = node.m_Position;
                        node.m_ID = positionalKey;
                        m_Ship.m_NodeList.emplace(positionalKey, node);
                    }
                }
                xLayer++;
            }
            xLayer = 0;
            yLayer++;
        }
        shipFile.close();
    } else {
        std::cout << "File not open\n";
    }


    unsigned int TESTCOUNTER = 0;
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    float hullVolumeTEST = 0;
    for (nodeIter i = m_Ship.m_NodeList.begin(); i != m_Ship.m_NodeList.end(); i++) {
        if (i->second.m_IsHull) {
            //Neighbour counter
            //3 = corner
            //2 = edge
            //1 = cross
            int nodeOrientationType = m_Ship.getNodePosition(i->first);

            //(8*0.5^3) - ((4*0.5^2)^2*3*)
            float hullVolume = (pow(nodeToNodeDistanceMetre, 2)*pow(0.5, nodeOrientationType - 1)*0.022225*nodeOrientationType);
            hullVolumeTEST += hullVolume; //delete after test
            //i->second.m_EmptyVolumePercentage = ((pow(nodeToNodeDistanceMetre, 3)*pow(0.5,nodeOrientationType))-(pow(nodeToNodeDistanceMetre, 2)*pow(0.5,nodeOrientationType-1))*m_Ship.hullThickness*nodeOrientationType);
            i->second.m_Volume = (pow(nodeToNodeDistanceMetre, 3)*pow(0.5, nodeOrientationType));
            i->second.m_MaxFloodableVolume = i->second.m_Volume - hullVolume; //volume that's floodable is volume of node without the space taken by the hull
            i->second.m_Mass = (hullVolume * i->second.m_Density);
            m_Ship.m_TotalVolume += i->second.m_Volume;
            m_Ship.m_ShipMass += i->second.m_Mass;
            //0.93325
        } else {
            i->second.m_Volume = pow(nodeToNodeDistanceMetre, 3);
            i->second.m_MaxFloodableVolume = i->second.m_Volume;
            i->second.m_Mass = i->second.m_Volume*i->second.m_Density;
            m_Ship.m_TotalVolume += i->second.m_Volume;
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
            + std::to_string(yLayer) + "-"
            + std::to_string(zLayer);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    if (x != 0 || y != 0 || z != 0) {
                        std::string keyCheck = std::to_string(xLayer + x) + "-"
                            + std::to_string(yLayer + y) + "-"
                            + std::to_string(zLayer + z);
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
    m_Ship.findOuterNodes();
    std::cout << "hullVolumeTotal: " << hullVolumeTEST << "\n";
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
    std::string checks[6] = { checkX1, checkY1, checkZ1, checkX2, checkY2, checkZ2 };

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
    if (xCount + yCount >= 4 || yCount + zCount >= 4 || xCount + zCount >= 4) {
        return 1;
    } else if (xCount < 2 && yCount < 2 && zCount < 2) {
        return 3;
    } else {
        return 2;
    }
    return -1;
}

Simulation::Controller::Controller(Camera &camera, std::string &fileName) {
    m_FileName = fileName;
    m_Renderer.setCamera(camera);
    m_World.loadWorld(m_FileName);
    m_Renderer.initialiseSceneRender();
    m_DeltaTime = 0.02f;
}

void Simulation::Controller::update() {
    m_TotalTimeElapsed += m_DeltaTime;
    float currentTime = glfwGetTime();
    m_LastTime = currentTime;
    m_Renderer.m_Camera->updateCameraSpeed(m_DeltaTime);
    updatePhysics();
    updateGraphics();
    
    if (((int)(m_TotalTimeElapsed)) != currentRoundedTime &&  ((int)(m_TotalTimeElapsed)) % 5 == 0) {
        placeIncrementalData();
        currentRoundedTime = ((int)(m_TotalTimeElapsed));
    }
}

// Credit: Luke Wren for spring physics
void Simulation::Controller::updatePhysics() {
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator floodingIter;
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator erasureIter;
    typedef std::map<std::string, PhysicsObjects::PhysicsSpring>::iterator springIter;
    std::map<std::string, PhysicsObjects::PhysicsObject> markedForErasure;
    /*
    if (floodingStart) {
        //std::cout << "-----------------------------------------------\n";
        //float waterFlowed = calculateFloodingAmount(m_DeltaTime)/m_FloodingList.size();
        for (floodingIter i = m_FloodingList.begin(); i != m_FloodingList.end(); i++) {
            float waterFlowed = i->second.m_MaxFloodableVolume * 0.01;
            i->second.flood(waterFlowed);
            //std::cout << i->first << ": FloodedPercentage: " << ((i->second.m_CurrentFloodedVolume) / (i->second.m_MaxFloodableVolume)) * 100 << "\n";
            if (i->second.m_CurrentFloodedVolume == i->second.m_MaxFloodableVolume) {
               markedForErasure.emplace(i->first, i->second);
            }
        }
        for (erasureIter i = markedForErasure.begin(); i != markedForErasure.end(); i++) {
            //addAdjacentNodes(i);
            m_FloodingList.erase(i->first);
        }
    }
    */

    //std::cout << "List: " << m_World.m_Ship.calculateList() << "\n";
    //std::cout << "Trim: " << m_World.m_Ship.calculateTrim() << "\n";
    std::cout << "numberOfNodesFlooded: " << m_World.m_Ship.numberOfNodesFlooded << "PercentageFlooded: " << (m_World.m_Ship.totalFloodedVolume/m_World.m_Ship.m_TotalVolume) * 100 << "\n";
    //GRAVITY IS CURRENTLY DISABLED IF YOU SEE NO Z MOVEMENT
    for (nodeIter i = m_World.m_Ship.m_NodeList.begin(); i != m_World.m_Ship.m_NodeList.end(); i++) {
        if (floodingStart) {
            if (i->second.m_FloodingAllowed) {
                //std::cout << "numNodesFlooded: " << m_World.m_Ship.numberOfNodesFlooded << "\n";
                float waterFlowed = (i->second.m_MaxFloodableVolume * 0.001);// / m_World.m_Ship.numberOfNodesFlooded;
                //float waterFlowed = calculateFloodingAmount(m_DeltaTime) / m_World.m_Ship.numberOfNodesFlooded;
                //std::cout << i->second.m_ID << ", currentFloodedVolume: " << i->second.m_CurrentFloodedVolume << "\n";
                m_World.m_Ship.totalFloodedVolume += i->second.flood(waterFlowed);
                //std::cout << "totalFloodedVolume: " << m_World.m_Ship.totalFloodedVolume << "\n";
                if (i->second.m_CurrentFloodedVolume == i->second.m_MaxFloodableVolume) {
                    if (!i->second.m_IsBulkhead) {
                        addAdjacentNodes(i);
                    }
                    i->second.m_FloodingAllowed = false;
                    m_World.m_Ship.numberOfNodesFlooded--;
                }
            }
        }
        if (i->second.m_GravityEnabled) {
            //std::cout << i->second.toString() << ",masss: " << i->second.m_Mass << "\n";
            i->second.applyForce(m_World.m_GravityAcceleration*i->second.m_Mass);
        }
        //Buoyancy
        if (i->second.m_Position.y <= m_World.m_Ocean.m_Position.y) {
            float percentageBelowWater = 1;
            float depthPercentageMax = 100;
            depthPercentageMax = (i->second.m_IsHull) ? 100 : 50; //calibrates better with surrounding air nodes because centre of surrounded ndoes is different thatn hull nodes
            if (abs(m_World.m_Ocean.m_Position.y - i->second.m_Position.y) <= depthPercentageMax) {
                percentageBelowWater = abs(m_World.m_Ocean.m_Position.y - i->second.m_Position.y) / depthPercentageMax;
            }


            //std::cout << i->first << ": FloodedPercentage: " << ((i->second.m_CurrentFloodedVolume) / (i->second.m_MaxFloodableVolume)) * 100 << "\n";
            //std::cout << "buoyant: " << glm::to_string(m_World.calibrate(m_World.m_GravityAcceleration*i->second.m_EmptyVolumePercentage*m_World.m_WaterDensity, 100, m_World.nodeToNodeDistanceMetre)*percentageBelowWater*(-1.0f)) << "\n";
            i->second.applyForce(m_World.m_GravityAcceleration*(i->second.m_Volume-i->second.m_CurrentFloodedVolume)*(m_World.m_WaterDensity)*percentageBelowWater*(-1.0f));
            //std::cout << "velocity: " << glm::to_string(i->second.m_Velocity) << "\n";


            //drag
            //i->second.applyForce((m_World.m_WaterDensity*(pow(m_World.nodeToNodeDistanceMetre,2))*(i->second.m_Velocity*i->second.m_Velocity)*0.5f*(-1.0f))); //actual
            //std::cout << "Mass of Node : " << i->second.m_Mass << "\n";
            //std::cout << i->second.toString() << ", Velocity below: " << glm::to_string(i->second.m_Velocity) << "\n";
            i->second.applyForce((glm::abs(i->second.m_Velocity)*i->second.m_Velocity)*(-20.0f)); //test drag, just used for slowing down based on velocity



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
            float dampingAmount = pow(0.5f, m_DeltaTime);
            j->second.applyDamping(dampingAmount);
            if (k == 0) {
                j->second.updateRenderObject(false, 100.0f);
            }
        }
    }
}

void Simulation::Controller::addAdjacentNodes(std::map<std::string, PhysicsObjects::PhysicsObject>::iterator node) {
    int xLayer = 0;
    int yLayer = 0;
    int zLayer = 0;
    unsigned int counter = 0;
    std::string xLayerString = "";
    std::string yLayerString = "";
    std::string zLayerString = "";
    for (char c : node->first) {
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

    std::map<std::string, PhysicsObjects::PhysicsObject>::iterator comparableNodeListIter;
    std::stringstream(xLayerString) >> xLayer;
    std::stringstream(yLayerString) >> yLayer;
    std::stringstream(zLayerString) >> zLayer;

    //What determines if water flows.
    //Once node is flooded, adjacent nodes that are not already filling/filled start filling
    //
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                if (x != 0 || y != 0 || z != 0) {
                    std::string keyCheck = std::to_string(xLayer + x) + "-"
                        + std::to_string(yLayer + y) + "-"
                        + std::to_string(zLayer + z);
                    comparableNodeListIter = m_World.m_Ship.m_NodeList.find(keyCheck);
                    if (comparableNodeListIter != m_World.m_Ship.m_NodeList.end() && comparableNodeListIter->second.m_CurrentFloodedVolume != comparableNodeListIter->second.m_MaxFloodableVolume && !comparableNodeListIter->second.m_FloodingAllowed) {
                        comparableNodeListIter->second.m_FloodingAllowed = true;
                        m_World.m_Ship.numberOfNodesFlooded++;
                    }
                }
            }
        }
    }
}

float Simulation::Controller::calculateFloodingAmount(float deltaTime) {
    float waterlineDepth = 0;
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    unsigned int counterForFirst = 0; //so first node will be assigned base
    for (nodeIter i = m_World.m_Ship.m_NodeList.begin(); i != m_World.m_Ship.m_NodeList.end(); i++) {
        if (i->second.m_FloodingAllowed) {
            float nodeDistance = glm::length(m_World.m_Ocean.m_Position - i->second.m_Position);
            if (counterForFirst == 0) {
                waterlineDepth = nodeDistance;
            } else {
                if (nodeDistance < waterlineDepth) {
                    waterlineDepth = nodeDistance;
                }
            }
            counterForFirst++;
        }
    }

    //Torricelli's law
    // velocity of water through a hole = sqrt(2*g*h)
    // calibrated water height = waterlineDepth / 100 * nodeToNodeDistanceMetre
    // amount of water = area * velocity * dt
    float waterVelocity = sqrt(2*glm::length(m_World.m_GravityAcceleration) * (waterlineDepth/100)*m_World.nodeToNodeDistanceMetre);
    float area = 0.0025;
    return area * waterVelocity * deltaTime;
}

void Simulation::Controller::startFlooding(PhysicsObjects::PhysicsObject &physicsObject) {
    floodingStart = true;
    physicsObject.m_FloodingAllowed = true;
    m_World.m_Ship.numberOfNodesFlooded++;
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


void Simulation::Ship::findOuterNodes() {
    typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
    PhysicsObjects::PhysicsObject *bowNode = &m_NodeList.begin()->second;
    PhysicsObjects::PhysicsObject *sternNode = &m_NodeList.begin()->second;
    PhysicsObjects::PhysicsObject *portNode = &m_NodeList.begin()->second;
    PhysicsObjects::PhysicsObject *starboardNode = &m_NodeList.begin()->second;

    for (nodeIter i = m_NodeList.begin(); i != m_NodeList.end(); i++) {
        if (i->second.m_Position.y == 0) {
            if (i->second.m_Position.x < bowNode->m_Position.x) {
                bowNode = &i->second;
            }
            if (i->second.m_Position.z < portNode->m_Position.z) {
                portNode = &i->second;
            }
        }
    }
    for (nodeIter i = m_NodeList.begin(); i != m_NodeList.end(); i++) {
        if (i->second.m_Position.y == 0) {
            if (i->second.m_Position.x > sternNode->m_Position.x && i->second.m_Position.z == bowNode->m_Position.z) {
                sternNode = &i->second;
            }
            if (i->second.m_Position.z > starboardNode->m_Position.z  && i->second.m_Position.x == portNode->m_Position.x) {
                starboardNode = &i->second;
            }
        }
    }
    m_OuterNodes.push_back(bowNode);
    m_OuterNodes.push_back(sternNode);
    m_OuterNodes.push_back(portNode);
    m_OuterNodes.push_back(starboardNode);
}

float Simulation::Ship::calculateTrim() {
    glm::vec3 difference = m_OuterNodes.at(1)->m_Position - m_OuterNodes.at(0)->m_Position;
    glm::vec3 flat(difference.x, 0, difference.z);
    //std::cout << "node1: " << m_OuterNodes.at(1)->toString() << "node 2: " << m_OuterNodes.at(0)->toString() << "\n";
    //std::cout << "difference: " << glm::to_string(difference) << "flat: " << glm::to_string(flat) << "trim: ";
    std::cout << acosf(glm::dot(difference, flat) / (glm::length(difference)*glm::length(flat))) * 180 / 3.141f << "\n";
    return acosf(glm::dot(difference,flat)/(glm::length(difference)*glm::length(flat)))*180/3.141f;
}
float Simulation::Ship::calculateList() {
    glm::vec3 difference = m_OuterNodes.at(3)->m_Position - m_OuterNodes.at(2)->m_Position;
    glm::vec3 flat(difference.x, 0, difference.z);
    std::cout << "node1: " << m_OuterNodes.at(1)->toString() << "node 2: " << m_OuterNodes.at(0)->toString() << "\n";
    std::cout << "difference: " << glm::to_string(difference) << "flat: " << glm::to_string(flat) << "list: ";
    std::cout << acosf(glm::dot(difference, flat) / (glm::length(difference)*glm::length(flat))) * 180 / 3.141f << "\n";
    return acosf(glm::dot(difference, flat) / (glm::length(difference)*glm::length(flat))) * 180 / 3.141f;
}

float Simulation::Ship::calculateMaximumWaterAllowed(glm::vec3 &gravity, float &waterDensity) {
    //ship can no longer stay afloat if it's weight is larger than the buoyant force
    glm::vec3 weight = glm::abs(m_ShipMass * gravity);
    glm::vec3 maxBuoyancy = glm::abs(m_TotalVolume * waterDensity * gravity);
    return glm::length(maxBuoyancy - weight) / (waterDensity * glm::length(gravity) * 1.0f);
}

void Simulation::Controller::placeStartingData(){
    //copied data https://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way
    std::ifstream  shapeDataFile("Ships/" + m_FileName + ".txt", std::ios::binary);
    std::ofstream  destinationfile("Results/"+m_FileName+"-Results.txt", std::ios::binary);
    destinationfile << shapeDataFile.rdbuf();
    destinationfile.close();
    std::ofstream file;
    file.open("Results/" + m_FileName + "-Results.txt", std::ios_base::app);
    file << "\n";
    file << "Total ship volume: " << std::to_string(m_World.m_Ship.m_TotalVolume) + "\n"; 
    file << "Maximum Water Intake: " << std::to_string(m_World.m_Ship.calculateMaximumWaterAllowed(m_World.m_GravityAcceleration, m_World.m_WaterDensity)) << "\n";
    file << "Hole Node: " << m_World.m_Ship.floodingNodeID << "\n";
    file << "Water Density: " << m_World.m_WaterDensity << "\n";
    file.close();
}

void Simulation::Controller::placeIncrementalData() {
    std::ofstream file;
    file.open("Results/" + m_FileName + "-Results.txt", std::ios_base::app);
    file << "-----------------------------------------------------------------------------------------------------\n";
    file << "Time: " << m_TotalTimeElapsed << "\n";
    file << "List: " << m_World.m_Ship.calculateList() << "\n";
    file << "Trim: " << m_World.m_Ship.calculateTrim() << "\n";
    file << "Current Flooded Volume: " << m_World.m_Ship.totalFloodedVolume << "\n";
    file.close();
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