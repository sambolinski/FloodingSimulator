#include "Renderer.h"
#include <string>
#include <iostream>
Renderer::Renderer() {}

//used for rendering cube and springs
void Renderer::render(std::map<std::string,PhysicsObjects::PhysicsObject> &physicsObjects){
    std::string previousType = "NaN";
    std::map<std::string, PhysicsObjects::PhysicsObject>::iterator objectPointer;
    for (objectPointer = physicsObjects.begin(); objectPointer != physicsObjects.end(); objectPointer++) {
        //since dhaders are identical for nodes, only load the shader once per rendering
        if (objectPointer->second.m_Type == previousType) {
        } else {
            m_Shader.deleteShader();
            m_Shader.loadShader(objectPointer-> second.m_Object.m_VertexShaderSource, objectPointer->second.m_Object.m_FragmentShaderSource);
            m_Shader.use();
        }
        int vertexColourLocation = glGetUniformLocation(m_Shader.m_ID, "colour");
        glUniform4f(vertexColourLocation, objectPointer->second.m_Object.m_Colour.x, objectPointer->second.m_Object.m_Colour.y, objectPointer->second.m_Object.m_Colour.z, objectPointer->second.m_Object.m_Colour.a);

        //Matrices so everything appears in window properly, these are passed to the dhader
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        
        view = glm::lookAt(m_Camera->m_CameraPos, m_Camera->m_CameraPos + m_Camera->m_CameraFront, m_Camera->m_CameraUp);
        projection = glm::perspective(glm::radians(m_Camera->m_Fov), (float)1500 / (float)900, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(m_Shader.m_ID, "model");
        unsigned int viewLoc = glGetUniformLocation(m_Shader.m_ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        m_Shader.setValue("projection", projection);

        //Applying translations/scaling to matrix
        model = glm::scale(model, objectPointer->second.m_Scale);
        model = glm::translate(model, objectPointer->second.m_Position);

        m_Shader.setValue("model", model);
        objectPointer->second.m_Object.render(m_Shader);

        previousType = (objectPointer->second.m_Type);
    }
}
//used to render the springs
void Renderer::renderSprings(std::map<std::string, PhysicsObjects::PhysicsSpring> &springs) {
    
    std::map<std::string, PhysicsObjects::PhysicsSpring>::iterator objectPointer;
    for (objectPointer = springs.begin(); objectPointer != springs.end(); objectPointer++) {
        if (objectPointer == springs.begin()) {
            m_Shader.deleteShader();
            m_Shader.loadShader(objectPointer->second.m_RenderObject.m_VertexShaderSource, objectPointer->second.m_RenderObject.m_FragmentShaderSource);
            m_Shader.use();
        }

        int vertexColourLocation = glGetUniformLocation(m_Shader.m_ID, "colour");
        glm::vec4 mixedColour = (objectPointer->second.m_FirstObject->m_Object.m_Colour + objectPointer->second.m_SecondObject->m_Object.m_Colour)*0.5f;
        glUniform4f(vertexColourLocation, mixedColour.x, mixedColour.y, mixedColour.z, mixedColour.a);

        //Matrices so everything appears in window properly
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        view = glm::lookAt(m_Camera->m_CameraPos, m_Camera->m_CameraPos + m_Camera->m_CameraFront, m_Camera->m_CameraUp);
        projection = glm::perspective(glm::radians(m_Camera->m_Fov), (float)1500 / (float)900, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(m_Shader.m_ID, "model");
        unsigned int viewLoc = glGetUniformLocation(m_Shader.m_ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        m_Shader.setValue("projection", projection);

        //Applying translations/scaling to matrix
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, (objectPointer->second.m_FirstObject->m_Position));

        m_Shader.setValue("model", model);
        objectPointer->second.m_RenderObject.render(m_Shader);
    }
}
void Renderer::initialiseSceneRender() {
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Clears the scene for each render
void Renderer::clear() {
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
}
