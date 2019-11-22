#include "Controller.h"
#include <iostream>
#include <fstream>
#include <string>

void Simulation::World::loadWorld() {
    m_OtherRenderedObjects.push_back(m_Ocean);
    loadShip();
}
void Simulation::World::loadShip() {
    const unsigned int LAYER_HEIGHT = 50;
    const unsigned int LAYER_WEIGHT = 50;
    std::string line;
    std::ifstream shipFile;
    shipFile.open("Ships/ship.txt");
    unsigned int xLayer = 0;
    unsigned int yLayer = 0;
    unsigned int zLayer = 0;
    glm::vec4 colour;
    if (shipFile.is_open()) {
        while(getline(shipFile, line)){
            for (char& c : line) {
                if (c == '-') {
                    yLayer = -1;
                    zLayer++;
                }
                if (c!='0' && c!= ' ') {
                    if (c == 'S') {
                        colour = glm::vec4(0.310f + ((float)xLayer)/50, 0.318f, 0.329f, 1.0f);
                    } else if (c == 'A') {
                        colour = glm::vec4(0.839f, 0.839f, 0.839f, 1.0f);
                    }
                    if (c != '-') {
                        Node node;
                        node.m_Object.m_Colour = colour;
                        node.m_Position = glm::vec3(yLayer, zLayer, xLayer);
                        m_Ship.push_back(node);
                    }
                }
                xLayer++;
            }
            xLayer = 0;
            yLayer++;
        }
        std::cout << m_Ship.size();
    } else {
        std::cout << "FILE NOT OPEN\n";
    }
}

Simulation::Controller::Controller(Camera &camera) {
    m_Renderer.setCamera(camera);
    m_World.loadWorld();
    m_Renderer.initialiseSceneRender();
}

void Simulation::Controller::update() {
    float currentTime = glfwGetTime();
    m_DeltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;
    m_Renderer.m_Camera->updateCameraSpeed(m_DeltaTime);
    updateGraphics();
}

void Simulation::Controller::updatePhysics() {
    typedef std::vector<PhysicsObject>::iterator iter;
    for (iter i = m_World.m_Ship.begin(); i < m_World.m_Ship.end() && i->m_PhysicsEnabled; i++) {
        glm::vec3 newPosition = i->m_Position + (i->m_Velocity * m_DeltaTime) + (i->m_Acceleration)*(m_DeltaTime*m_DeltaTime*0.5f);
        glm::vec3 newAcceleration = m_World.m_GravityAcceleration;
        glm::vec3 newVelocity = i->m_Velocity + (i->m_Acceleration + newAcceleration)*(m_DeltaTime*0.5f);
        i->m_Position = newPosition;
        i->m_Velocity = newVelocity;
        i->m_Acceleration = newAcceleration;
    }
}

glm::vec3 Simulation::Controller::applyForce() {
    return glm::vec3(0, 0, 0);
}

void Simulation::Controller::updateGraphics() {
    m_Renderer.clear();
    m_Renderer.render(m_World.m_OtherRenderedObjects);
    m_Renderer.render(m_World.m_Ship);
}