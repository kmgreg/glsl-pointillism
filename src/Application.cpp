/*
@authors = Lauren Cole, Kurt Gregorek
entry point for painterly particle system assignment
gets keyboard input and calls update functions for particle system and camera
*/

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
#include "Camera.h"
#include "PainterlyParticleSystem.h"
#include "Random.h"

/**
get mouse position to update particle system origin. not used for Painterly particle system
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

/**
polls WASDQE for camera movement. returns a movement vector in xyz directions. 
values in vector are all 1, 0 or -1 to indicate direction of movement
these values are modulated inside the camera class based on 
the camera's speed member variable
*/
glm::vec3 pollKeysToMoveCamera(GLFWwindow* window) {
    glm::vec3 movement = glm::vec3(0.0f);
    int state;
    state = glfwGetKey(window, GLFW_KEY_W);
    if (state == GLFW_PRESS) {
        movement.z = movement.z - 1.0f;
    }
    state = glfwGetKey(window, GLFW_KEY_S);
    if (state == GLFW_PRESS) {
        movement.z = movement.z + 1.0f;
    }
    state = glfwGetKey(window, GLFW_KEY_A);
    if (state == GLFW_PRESS) {
        movement.x = movement.x - 1.0f;
    }
    state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS) {
        movement.x = movement.x + 1.0f;
    }
    state = glfwGetKey(window, GLFW_KEY_Q);
    if (state == GLFW_PRESS) {
        movement.y = movement.y + 1.0f;
    }
    state = glfwGetKey(window, GLFW_KEY_E);
    if (state == GLFW_PRESS) {
        movement.y = movement.y - 1.0f;
    }
    return movement;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Pointillism", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) std::cout << "glewInit() Error!" << std::endl;

    /*initilize render loop variables*/
    PainterlyParticleSystem pps = PainterlyParticleSystem(100, "res/shaders/Pointillism.shader", "res/objects/teapot.obj", "res/shaders/Pointillism.geom");
    
    //good scaling for bunny
    /*pps.setTransformationMatrix(
        glm::mat4(6.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 6.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 6.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f)
    );*/
    //good scaling for teapot
    pps.setTransformationMatrix(
        glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f)
    );
    //pps.setTransformationMatrix(glm::mat4(1.0f));
    pps.getShader().bind();
    Renderer renderer;
    Camera camera;
    
    /*any additional render settings here*/
    renderer.enableBlend();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2);
    glEnable(GL_POINT_SMOOTH);
   
    /*settings for camera movement speed, TODO this should be handled in the camera class eventually*/
    float cameraSpeed = 0.01;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*RENDERING CODE */
        renderer.clear();
        camera.moveCamera(pollKeysToMoveCamera(window));
        camera.onUpdate(pps.getShader()); //call the camera update first because it needs to set view and projection matrices before the render
        pps.onUpdate(); //makes the draw call

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

