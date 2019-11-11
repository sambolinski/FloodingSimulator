#ifndef USERINPUT_H
#define USERINPUT_H
#include <GLFW/glfw3.h>
#include "Graphics/Camera.h"
class UserInput{
public:
    GLFWwindow *m_Window;
    UserInput(){}
    UserInput(GLFWwindow *window) :m_Window(window){}
    void processInput(Camera &camera);
};
#endif //!USERINPUT_H