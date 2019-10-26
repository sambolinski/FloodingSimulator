#include <GLFW/glfw3.h>
#include <iostream>


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

    //main game loop, used to update world and to update drawing
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}