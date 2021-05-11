#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
//#include "ParticleSystem.h"
#include "PainterlyParticleSystem.h"
#include "Random.h"

/**
get mouse position to update particle system origin
*/
glm::vec3 getMouseCoordinates(double x, double y, int windowx, int windowy, glm::mat4 proj, glm::mat4 view)
{
    glm::mat4 matrix = view * proj;
    matrix = glm::inverse(matrix);
    float worldx = -1.0f + ((x / (float)windowx) * 2.0f);
    float worldy = 1.0f - ((y / (float)windowy) * 2.0f);
    glm::vec4 ret = glm::vec4(worldx, worldy, 0.0f, 1.0f);
    ret = ret * matrix;
    glm::vec3 no = glm::vec3(ret[0], ret[1], ret[2]);
    return no;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, -1.0f); 
    glm::mat4 model = glm::mat4(1.0f);   

    /*initilize render loop variables*/
    double xpos = 0.0f;
    double ypos = 0.0f;
    int size = 60000;
   // ParticleSystem ps = ParticleSystem(size, glm::vec3(0.0f, 0.0f, 0.0f), "   res/shaders/Basic.shader", "res/shaders/Rainbow.compute");
    PainterlyParticleSystem pps = PainterlyParticleSystem(100, "res/shaders/Blue.shader", "res/Shaders/Smoke.compute", "res/objects/bunny.obj", "res/shaders/shader.geom");
    pps.setTransformationMatrix(
        glm::mat4(6.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 6.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 6.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f)
    );
    glm::mat4 view = glm::mat4(1.0);
    unsigned int pps_shaderID = pps.getShaderID();
    glUseProgram(pps_shaderID);
    GLuint projLoc = glGetUniformLocation(pps_shaderID, "u_proj");
    GLuint viewLoc = glGetUniformLocation(pps_shaderID, "u_view");
    GLuint modLoc = glGetUniformLocation(pps_shaderID, "u_model");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    Renderer renderer;

    

    /*any additional render settings here*/
   
    renderer.enableBlend();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(20);
   // glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
   // glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
   

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*RENDERING CODE */
        renderer.clear();
        pps.onUpdate();

       /* ///* update particle system   */
        /*int LMBState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (LMBState == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            glm::vec3 worldcoordspos = getMouseCoordinates(xpos, ypos, 640, 480, proj, view);
            std::cout << "Xnew: " << worldcoordspos[0] << " Y: " << worldcoordspos[1] << std::endl;
            ps.updateOrigin(worldcoordspos[0], worldcoordspos[1]);
            ps.emit(25);
        } 
        ps.onUpdate();*/
        
        /*END RENDER CODE*/

        //EXPERIMENTAL
      /*  shader.setUniform4fv("u_color", color);*/
        /*shader.setUniformMat4f("u_model", model);
        shader.setUniformMat4f("u_proj", proj);
        shader.setUniformMat4f("u_view", view);
        shader.setUniform3fv("u_lerpColors", 6, lerpColors);
        renderer.draw(va, ib, shader);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

