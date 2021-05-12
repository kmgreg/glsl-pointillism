/*
@author = Lauren Cole, Kurt Gregorek
class to represent a pointillism particle system
it is of note the system is static because particles are papermached on to an object mesh to represent pointillism
*/

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
#include <glm/gtc/type_ptr.hpp>


#pragma once

struct PaintParticle {
	glm::vec4 position;
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
	Renderer renderer;
	glm::mat4 m_transformationMatrix;
	int m_size;
	std::vector<PaintParticle> m_particles;
	glm::vec3 particleScale = glm::vec3(0.01f, 0.01, 0.0f);
	std::vector<unsigned int> m_masterIndexBuffer;
	std::vector<PaintParticle> m_masterVBO;
	unsigned int shaderId;
	glm::vec4 m_objColor;
	float minAreaLeeway;

	float getTriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c);

public:
	PainterlyParticleSystem(int size, std::string vfshaderfilepath, std::string objectfilepath, std::string geoshaderfilepath);
	void initializeArray(std::string objectfilepath); //called in constructor will initialize particle data
	void generateMasterIndexArray(); //called in constructor, will initialize index buffer 
	unsigned int getShaderID(); //return shader program
	void batchRenderSystem(); //make the draw call for all the particles
	void onUpdate(); //we will update the particles to rotate towards the camera every frame
	void setTransformationMatrix(glm::mat4 tMat);
	Shader& getShader();
	std::vector<PaintParticle> addPoints(float minArea, glm::vec3 a, glm::vec3 b, glm::vec3 c);
	PaintParticle getTriangleCenterAsPaintParticle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	void setMinAreaLeeway(float leeway);
	void setObjColor(glm::vec4 color);
	glm::vec4 getObjColor();
};

