#include "Controller.h"
#include <iostream>
void Simulation::World::loadShip() {
    for (int i = 0; i < 10; i++) {
        Node node;
        m_Ship.push_back(node);
    }
    m_Ship.at(0).m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Ship.at(1).m_Position = glm::vec3(2.0f, 5.0f, -15.0f);
    m_Ship.at(2).m_Position = glm::vec3(-1.5f, -2.2f, -2.5f);
    m_Ship.at(3).m_Position = glm::vec3(-3.8f, -2.0f, -12.3f);
    m_Ship.at(4).m_Position = glm::vec3(2.4f, -0.4f, -3.5f);
    m_Ship.at(5).m_Position = glm::vec3(-1.7f, 3.0f, -7.5f);
    m_Ship.at(6).m_Position = glm::vec3(1.3f, -2.0f, -2.5f);
    m_Ship.at(7).m_Position = glm::vec3(1.5f, 2.0f, -2.5f);
    m_Ship.at(8).m_Position = glm::vec3(1.5f, 0.2f, -1.5f);
    m_Ship.at(9).m_Position = glm::vec3(-1.3f, 1.0f, -1.5f);
}

Simulation::Controller::Controller(UserInput &userInput) {
    m_UserInput = userInput;
    m_World.loadShip();
    m_Renderer.initialiseSceneRender();
}

void Simulation::Controller::update() {
    float currentTime = glfwGetTime();
    std::cout << "currentTime: " << currentTime << "\n";
    m_DeltaTime = currentTime - m_LastTime;
    std::cout << "deltaTime: " << m_DeltaTime << "\n";
    std::cout << "-------------------------" << "\n";
    m_LastTime = currentTime;
    m_Renderer.m_Camera.updateCameraSpeed(m_DeltaTime);
    updatePhysics();
    updateUserInput();
    updateGraphics();
}

void Simulation::Controller::updatePhysics() {

}

void Simulation::Controller::updateUserInput() {
    m_UserInput.processInput(m_Renderer.m_Camera);
}

void Simulation::Controller::updateGraphics() {
    m_Renderer.clear();
    m_Renderer.render(m_World.m_Ship);
}