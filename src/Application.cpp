/*
@authors = Lauren Cole, Kurt Gregorek
entry point for painterly particle system assignment
gets keyboard input and calls update functions for particle system and camera

NOTE: an expensive recursive function is used to initialize the particle systems. 
it is only called in the particle system constructor, not per frame. 
the application may take a moment to load intially, but should have a reasonable framerate
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
#include "Light.h"
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
    
    // pps.getShader().bind();

   

    


    /*initilize particle systems variables*/
    PainterlyParticleSystem teapotPS = PainterlyParticleSystem("res/shaders/Pointillism.shader", 
        "res/objects/teapot.obj", "res/shaders/Pointillism.geom", 50);
    PainterlyParticleSystem bunnyPS = PainterlyParticleSystem("res/objects/bunny.obj", teapotPS.getShader(), 100);
    //settings for bunny
    glm::mat4 bunnyModelMatrix = glm::mat4(1.0f);
    bunnyModelMatrix = glm::translate(bunnyModelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    bunnyModelMatrix = glm::scale(bunnyModelMatrix, glm::vec3(6.0f, 6.0f, 6.0f));
    bunnyPS.setTransformationMatrix(bunnyModelMatrix);

    glm::vec4 vBlue = { 0,0.1,1,1 };
    glm::vec4 vDBlue = { 0,0.1,1,1 };
    glm::vec4 vWhite = { 1,1,1,1 };
    glm::vec3 kCoeff = { 1,.1,1 };
    bunnyPS.setPhongVariables(vBlue, vDBlue, vWhite, 20, kCoeff);

    glm::vec4 vYellow = { 1,1,0,1 };
    glm::vec4 vLYellow = { 0.7,0.7,0.5,1 };
    glm::vec4 lightPos = { 5,5,4,1 };

    Light ls = Light(bunnyPS.getShader(), lightPos, vYellow, vLYellow);

    //settings for teapot
    glm::mat4 teapotModelMatrix = glm::mat4(1.0f);
    teapotModelMatrix = glm::scale(teapotModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
    teapotModelMatrix = glm::translate(teapotModelMatrix, glm::vec3(-4.0f, 0.0f, 0.0f));
    teapotPS.setObjColor(glm::vec4(0.5f, 0.3f, 0.4f, 1.0f));
    teapotPS.setTransformationMatrix(teapotModelMatrix);



    //they use the same instance of Shader we can bind the shader from whatever object
    bunnyPS.getShader().bind();
    Renderer renderer;
    Camera camera;
    
    /*additional render settings*/
    renderer.enableBlend();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2); //point size will be the same for all objects in scene because thats how it be in paintings
    glEnable(GL_POINT_SMOOTH);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*RENDERING CODE */
        renderer.clear();
        camera.moveCamera(pollKeysToMoveCamera(window));
        ls.setAndApplyToNewShader(bunnyPS.getShader());
        camera.onUpdate(bunnyPS.getShader()); //call the camera update first because it needs to set view and projection matrices before the render
        teapotPS.onUpdate(); //makes the draw call
        bunnyPS.onUpdate();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

