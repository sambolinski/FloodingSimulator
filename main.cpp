#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Graphics/Renderer.h"
#include "Physics/Node.h"
#include "Controller.h"
#include <glm/gtx/string_cast.hpp>

/*The OpenGL graphics for this project makes extensive use of the tutorial by Joey de Vries
found here: https://learnopengl.com/
My work uses and adapts Joey's work allowed under CC BY 4.0 license
https://creativecommons.org/licenses/by/4.0/legalcode
Joey's Twitter: https://twitter.com/JoeyDeVriez
*/

//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Window
//Author: Joey de Vries
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Variables taken directly
//Begin
const unsigned int WINDOW_WIDTH = 1500;
const unsigned int WINDOW_HEIGHT  = 900;
double lastX = WINDOW_WIDTH/2;
double lastY = WINDOW_HEIGHT/2;
bool firstMouse = true;





//Method taken directly from the tutorial
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//End

Camera camera;

//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Camera
//Author: Joey de Vries
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Method taken directly from the tutorial except for keys E, R, T, P
//Begin
void processInput(GLFWwindow* window, Simulation::Controller &controller) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.m_CameraPos += camera.m_CameraSpeed * camera.m_CameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.m_CameraPos -= camera.m_CameraSpeed * camera.m_CameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.m_CameraPos -= glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) * camera.m_CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.m_CameraPos += glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) *camera.m_CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.m_CameraPos += camera.m_CameraUp *camera.m_CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.m_CameraPos -= camera.m_CameraUp *camera.m_CameraSpeed;
    }
    //End
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        //first node full water applied
        controller.m_World.m_Ship.m_NodeList.at("0-0-0").flood(controller.m_World.m_Ship.m_NodeList.at("0-0-0").m_MaxFloodableVolume);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        //all node full applied
        typedef std::map<std::string, PhysicsObjects::PhysicsObject>::iterator nodeIter;
        for (nodeIter i = controller.m_World.m_Ship.m_NodeList.begin(); i != controller.m_World.m_Ship.m_NodeList.end(); i++) {
           i->second.flood(i->second.m_MaxFloodableVolume);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        controller.incrementalFlooding = (controller.incrementalFlooding == true) ? false : true;
    }

    //starts the flooding at the specified node
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (controller.floodingStart == false) {
            controller.startFlooding(controller.m_World.m_Ship.m_NodeList.at(controller.m_World.m_Ship.floodingNodeID));
            controller.placeStartingData();
        }
    }
}

//#CONTRIBUTED CODE
//url: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
//Author: Joey de Vries
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Method taken directly
//Begin
void processMouseInput(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera.m_Yaw += xOffset;
    camera.m_Pitch += yOffset;

    //max pitch stops camera going upside down
    if (camera.m_Pitch > 89.0f)
        camera.m_Pitch = 89.0f;
    if (camera.m_Pitch < -89.0f)
        camera.m_Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(camera.m_Yaw)) * cos(glm::radians(camera.m_Pitch));
    front.y = sin(glm::radians(camera.m_Pitch));
    front.z = sin(glm::radians(camera.m_Yaw)) * cos(glm::radians(camera.m_Pitch));
    camera.m_CameraFront = glm::normalize(front);
}
//End


// main function sets up main window using GLFW
//#CONTRIBUTED CODE
//url: https://learnopengl.com/Getting-started/Hello-Window
//Author: Joey de Vries
//Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
//Comment: Methods to get window set up, glfw calls and GLAD calls taken from tutorial denoted by begin and end. Rest is made by me
int main() {
    //Begin
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
 
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flooding Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);

    //Initialises GLAD and returns -1 if it fails
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }
    //End

    //Getting the simulation name
    //////////////////////////////////////////////////////////////////////
    std::ifstream simulationFile;

    simulationFile.open("simulation.txt");
    std::string buffer;
    std::string fileLine;
    while (std::getline(simulationFile, fileLine)) {
        buffer += fileLine;
    }
    std::string fileName = buffer;
    ///////////////////////////////////////////////////////////////////////


    Simulation::Controller controller(camera, fileName);
    //Begin FROM TUTORIAL
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //allows drawing in glfw context whilst resizing
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); 
    glfwSetCursorPosCallback(window, processMouseInput);
    //End FROM TUTORIAL

    controller.startFlooding(controller.m_World.m_Ship.m_NodeList.at(controller.m_World.m_Ship.floodingNodeID));
    controller.placeStartingData();

    //main game loop, used to update world and to update drawing
    while (!glfwWindowShouldClose(window) && !controller.m_SimulationEnded) {
        processInput(window, controller);
        controller.update();
        //Begin FROM TUTORIAL
        glfwSwapBuffers(window);
        glfwPollEvents();
        //END FROM TUTORIAL
    }
    //Begin FROM TUTORIAL
    glfwTerminate();
    //End  FROM TUTORIAL
    return 0;
}