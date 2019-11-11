#include "Renderer.h"
#include <string>
#include <iostream>
Renderer::Renderer() {}

void Renderer::render(std::vector<PhysicsObject> &physicsObjects){
    std::string previousType = "NaN";
    std::vector<PhysicsObject>::iterator objectPointer;
    for (objectPointer = physicsObjects.begin(); objectPointer < physicsObjects.end(); objectPointer++) {
        if (objectPointer->m_Type == previousType) {
        } else {
            m_Shader.loadShader(objectPointer->m_Object.m_VertexShaderSource, objectPointer->m_Object.m_FragmentShaderSource);
        }

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        m_Camera.m_CameraSpeed = 2.5f*deltaTime;
        m_Shader.use();
        int vertexColourLocation = glGetUniformLocation(m_Shader.m_ID, "colour");
        glUniform4f(vertexColourLocation, (sin(glfwGetTime()) / 1.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f), 0.25f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.m_CameraUp);
        projection = glm::perspective(glm::radians(m_Camera.m_Fov), (float)1500 / (float)900, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(m_Shader.m_ID, "model");
        unsigned int viewLoc = glGetUniformLocation(m_Shader.m_ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        m_Shader.setValue("projection", projection);
        model = glm::translate(model, objectPointer->m_Position);

        m_Shader.setValue("model", model);
        objectPointer->m_Object.render(m_Shader);

        previousType = (objectPointer->m_Type);
    }
}
void Renderer::initialiseSceneRender() {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::clear() {
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
