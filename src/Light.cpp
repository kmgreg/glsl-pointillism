#include "Light.h"

Light::Light(Shader shaderObj, glm::vec4 position, glm::vec4 lightColor, glm::vec4 ambientColor)
{
	this->shader = shaderObj;
	this->pos = position;
	this->color = ambientColor;
	this->ambi = ambientColor;

	this->shader.setUniform4fv("lightPos", position);
	this->shader.setUniform4fv("lightColor", lightColor);
	this->shader.setUniform4fv("ambientLight", ambientColor);

}

void Light::setAndApplyToNewShader(Shader newShader)
{
	this->shader = newShader;

	this->shader.setUniform4fv("lightPos", this->pos);
	this->shader.setUniform4fv("lightColor", this->color);
	this->shader.setUniform4fv("ambientLight", this->ambi);
}

void Light::resetLightValues(glm::vec4 position, glm::vec4 lightColor, glm::vec4 ambientColor)
{
	this->pos = position;
	this->color = ambientColor;
	this->ambi = ambientColor;

	this->shader.setUniform4fv("lightPos", this->pos);
	this->shader.setUniform4fv("lightColor", this->color);
	this->shader.setUniform4fv("ambientLight", this->ambi);
}