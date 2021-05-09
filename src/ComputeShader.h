#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class ComputeShader
{
	std::string m_filepath;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;


public:
	ComputeShader(std::string& filepath);
	unsigned int getShaderID();
	unsigned int compileShader(const std::string& source);
	std::string parseShader(std::string& filepath);
	unsigned int createShader(const std::string& shader);
	void dispatch(int x, int y, int z);
	void use();
	void setUniform3fv(const std::string& name, int num, glm::vec3 vec[]);
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	int getUniformLocation(const std::string& name);
	
};

