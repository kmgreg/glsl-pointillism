#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
/*
* @author = Lauren Cole
class to abstract vertex and fragment shaders, based off onlien tutorials but functionality has been extended
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

enum class ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
};

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	std::string m_geomFilepath; //optional filepath for a geometry shader
public:
	Shader(const std::string& filename); //instantiate the shader with a file containing vertex and fragment
	Shader(const std::string& filename, const std::string& geomFilename); //instantiate with a geometry shader
	~Shader();
	Shader();
	void bind() const;
	void unbind() const;
	unsigned int getID();
	
	//set uniforms
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform1f(const std::string& name, float value);
	void setUniform1i(const std::string& name, int value);
	void setUniform3fv(const std::string& name, glm::vec3& vec);
	void setUniform3fv(const std::string& name, int num, glm::vec3 vec[]);
	void setUniformMat4f(const std::string& name, glm::mat4& matrix);
	void setUniform4fv(const std::string& name, glm::vec4& vec);


private:
	int getUniformLocation(const std::string& name);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	ShaderProgramSource ParseShader(const std::string& filepath, const std::string& geomFilepath);
	std::stringstream ParseGeometryShader(const std::string& geomFilepath);
};

