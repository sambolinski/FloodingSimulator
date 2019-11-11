#ifndef RENDERER_H
#define RENDERER_H
#include "Camera.h"
#include "Shader.h"
#include "../Physics/PhysicsObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Renderer {
public:
    //DECOUPLE THESE LATER///
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    /////////////////////////
    Camera m_Camera;
    Shader m_Shader;

    Renderer();
    void initialiseSceneRender();
    void render(std::vector<PhysicsObject> &physicsObjects);
    void clear();
};
#endif // !RENDERER_H


