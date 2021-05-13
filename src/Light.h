#include "Shader.h"
#include <glm/glm.hpp>

#pragma once

/*
	Class to represent light, loosely based on Lab 2
*/

class Light {
private:
	Shader shader;
	glm::vec4 pos = {0,0,0,1};
	glm::vec4 color = {1,1,1,1};
	glm::vec4 ambi = {0.5,0.5,0.5,0.5};

public:
	Light(Shader shaderObj, glm::vec4 position, glm::vec4 lightColor, glm::vec4 ambientColor);
	void setAndApplyToNewShader(Shader newShader);
	void resetLightValues(glm::vec4 position, glm::vec4 lightColor, glm::vec4 ambientColor);
};