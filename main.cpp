#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics/Renderer.h"
#include "Physics/Node.h"
#include "Controller.h"

/*The OpenGL graphics for this project makes extensive use of the tutorial by Joey de Vries
found here: https://learnopengl.com/
My work uses and adapts Joey's work allowed under CC BY 4.0 license
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// main function sets up main window using GLFW
int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
 
    GLFWwindow* window = glfwCreateWindow(1500, 900, "Flooding Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);
    //allows drawing in glfw context whilst resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initialises GLAD and returns -1 if it fails
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    UserInput userInput(window);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Simulation::Controller controller(userInput);
    //main game loop, used to update world and to update drawing
    while (!glfwWindowShouldClose(window)) {
        controller.update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}




