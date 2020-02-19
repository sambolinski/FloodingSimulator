#ifndef RENDERER_H
#define RENDERER_H
#include "Camera.h"
#include "Shader.h"
#include "../Physics/PhysicsObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>
//made using LearnOpenGL tutorial
class Renderer {
public:
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    Camera *m_Camera;
    Shader m_Shader;

    Renderer();
    void initialiseSceneRender();
    void render(std::map<std::string, PhysicsObjects::PhysicsObject> &physicsObjects);
    void clear();
    void renderSprings(std::map<std::string, PhysicsObjects::PhysicsSpring> &springs);
    Camera getCamera() { return *m_Camera; }
    void setCamera(Camera &camera) {
        m_Camera = &camera;
    }
};
#endif // !RENDERER_H


