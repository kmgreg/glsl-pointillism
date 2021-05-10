#include "Camera.h"

Camera::Camera()
{
	lookAt = glm::vec4(0.0, 0.0, 0.0, 0.0);
	position = glm::vec4(0.0, 0.0, 0.0, 0.0);
}

Camera::Camera(glm::vec4 pos)
{
	lookAt = glm::vec4(0.0, 0.0, 0.0, 0.0);
	position = pos;
}

Camera::Camera(glm::vec4 pos, glm::vec4 lookat)
{
	lookAt = lookat;
	position = pos;
}