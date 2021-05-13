#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/common.hpp>
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Random.h"
#include "ComputeShader.h"
#include "ShaderStorageBufferObject.h"
#include "Texture.h"
#include <vector>
#include <algorithm>
#include <array>
//#include <vendor\glm\ext\quaternion_common.hpp>
#include <glm/gtc/type_ptr.hpp>


#pragma once

struct PaintParticle {
	glm::vec4 position;
	glm::vec3 normal;
	//temp sorting, obviously will be sorting by distance from view position, but until we have a camera this is the equivalent of that
	bool operator < (const PaintParticle& particle) const
	{
		return (position.z < particle.position.z);
	}
};


class PainterlyParticleSystem
{
private:
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
	VertexBufferLayout layout;
	Shader shader;
	ComputeShader cShader;
	Renderer renderer;
	glm::mat4 m_transformationMatrix;


	int m_size;
	std::vector<PaintParticle> m_particles;
	glm::vec3 particleScale = glm::vec3(0.01f, 0.01, 0.0f);
	std::vector<unsigned int> m_masterIndexBuffer;
	std::vector<PaintParticle> m_masterVBO;
	unsigned int shaderId;

	glm::vec4 diff = {1,1,1,1};
	glm::vec4 ambi = {1,1,1,1};
	glm::vec4 spec = {1,1,1,1};
	GLfloat specExp;
	glm::vec3 kCoeff = {1,1,1};

public:
	PainterlyParticleSystem(int size, std::string vfshaderfilepath, std::string computefilepath, std::string objectfilepath, std::string geoshaderfilepath);
	void initializeArray(std::string objectfilepath); //called in constructor will initialize particle data
	void generateMasterIndexArray(); //called in constructor, will initialize index buffer 
	unsigned int getShaderID(); //return shader program
	void batchRenderSystem(); //make the draw call for all the particles
	void onUpdate(); //we will update the particles to rotate towards the camera every frame
	void setTransformationMatrix(glm::mat4 tMat);
	void setPhongVariables(glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular, GLfloat specExp, glm::vec3 kCoeff);
	Shader& getShader();

};

