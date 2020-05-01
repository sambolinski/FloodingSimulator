#include "Node.h"
Node::Node() {
    m_Type = "phys_Node";
    m_Mass = 0.5f;
    m_Scale = glm::vec3(0.01f, 0.01f, 0.01f);
    m_Object.m_Mode = 3;

    //#CONTRIBUTED CODE
    //url: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.3.coordinate_systems_multiple/coordinate_systems_multiple.cpp
    //Author: Joey de Vries
    //Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
    //Comment: Taken directly from the tutorial
    //Begin
    m_Object.m_Vertices = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    //End
    m_Object.m_Indices = {
         0,  1,  2,  
         0,  2,  3,  

         4,  5,  6,  
         4,  6,  7,   
         
         8,  9,  10, 
         8, 10,  11,  
         
         12, 13, 14, 
         12, 14, 15,  

         16, 17, 18, 
         17, 18, 19,  

         20, 21, 22, 
         20, 22, 23
    };
    //Begin
    //#CONTRIBUTED CODE
    //url: https://learnopengl.com/Getting-started/Shaders
    //Author: Joey de Vries
    //Licensing: CC BY 4.0 license https://creativecommons.org/licenses/by/4.0/legalcode
    //Comment: Method taken directly from the tutorial

    //Shaders from LearnOpenGL
    m_Object.m_VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
    m_Object.m_FragmentShaderSource= "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = colour;\n"
        "}\n\0";
    //End
    m_Object.InitialiseObjectData();
};