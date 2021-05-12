#include "Camera.h"


Camera::Camera()
{
	//default camera position is up in the air, back out of the screen, looking into the screen
	lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
	position = glm::vec3(0.0, 1.0, 3.0);
	cameraSpeed = 0.01;
}

Camera::Camera(glm::vec3 pos)
{
	lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
	position = pos;
	cameraSpeed = 0.01;
}

Camera::Camera(glm::vec3 pos, glm::vec3 lookat)
{
	lookAt = lookat;
	position = pos;
	cameraSpeed = 0.01;
}

glm::vec3 Camera::getPos()
{
	return position;
}

void Camera::setPos(glm::vec3 pos)
{
	position = pos;
}

//we are always expecting a vec3 with either -1, 0, or 1 in each slot
// ^^ set up in the keyboard callback in the Application.cpp
//then we multiply by a camera speed stored in the camera class to modulate the movement
void Camera::moveCamera(glm::vec3 translation)
{
	position = position + (translation*cameraSpeed);
}

///
/// This function sets up a frustum projection of the scene.
/// based from Carithers lab framework at RIT
/// @param shader- shader to which we are sending data
/// 
void Camera::setFrustum(Shader shader)
{
	glm::mat4 pmat = glm::frustum(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, -10.0f);
	//pmat = glm::mat4(1.0f);
    shader.setUniformMat4f("u_proj", pmat);
}

void Camera::setViewing(Shader shader)
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 vmat = glm::lookAt(position, position + lookAt, up);
	//glm::mat4 vmat = glm::mat4(1.0f);
	shader.setUniformMat4f("u_view", vmat);
}

void Camera::onUpdate(Shader shader)
{
    setViewing(shader);
    setFrustum(shader);
}