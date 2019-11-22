#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
class Camera {
public:
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_Fov = 45.0f;
    const float m_CameraSpeedCoefficient = 2.5;
    float m_CameraSpeed = 0;
    glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.5f, 5.0f);
    glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 m_LastPosition = glm::vec2(800.0f / 2.0f, 600.0f / 2.0f);

    Camera() {};
    void setYaw(float yaw) { m_Yaw = yaw; }
    void setPitch(float pitch) { m_Pitch = pitch; }
    void setFOV(float fov) { m_Fov = fov; }
    void updateCameraSpeed(float deltaTime) { m_CameraSpeed = m_CameraSpeedCoefficient * deltaTime; }
    glm::vec3 getCameraPos() { return m_CameraPos; }
    glm::vec3 getCameraFront() { return m_CameraFront; }
    glm::vec3 getCameraUp() { return m_CameraUp; }
    glm::vec2 getLastPosition() { return m_LastPosition; }
};
#endif
