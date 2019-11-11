#include "UserInput.h"
void UserInput::processInput(Camera &camera) {
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.m_CameraPos += camera.m_CameraSpeed * camera.m_CameraFront;
    }if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.m_CameraPos -= camera.m_CameraSpeed * camera.m_CameraFront;
    }if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.m_CameraPos -= glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) * camera.m_CameraSpeed;
    }if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.m_CameraPos += glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) *camera.m_CameraSpeed;
    }
}