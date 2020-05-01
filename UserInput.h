#ifndef USERINPUT_H
#define USERINPUT_H
#include <GLFW/glfw3.h>
#include "Graphics/Camera.h"
// NOT USED -- OUTDATED
class UserInput{
public:
    bool m_FirstMouse = true;
    double m_LastX;
    double m_LastY;
    GLFWwindow *m_Window;

    UserInput(){}
    UserInput(GLFWwindow *window) :m_Window(window){}
    void processInput(Camera &camera);
    void processMouseInput(double xpos, double ypos);
};
#endif //!USERINPUT_H