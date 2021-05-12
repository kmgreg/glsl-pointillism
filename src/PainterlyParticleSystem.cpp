/**
@author = Lauren Cole, Kurt Gregorek
Class to represent a particle system for the static painterly shader particles

**/


#include "PainterlyParticleSystem.h"
#include "OBJ_Loader.h"
PainterlyParticleSystem::PainterlyParticleSystem(int size, std::string vfshaderfilepath, std::string objectfilepath, std::string geoshaderfilepath) :
	vb(), shader(vfshaderfilepath, geoshaderfilepath), m_size(size), m_transformationMatrix(1.0f)
{
	//first we are going to make the vertex object array
	layout.push<float>(4); //push position
	initializeArray(objectfilepath);
	generateMasterIndexArray();
	vb.init(m_particles.data(), m_size * sizeof(PaintParticle), DYNAMIC_DRAW);
	va.addBuffer(vb, layout);
}

/*
generates index buffer for the system
*/
void PainterlyParticleSystem::generateMasterIndexArray()
{
	for (int i = 0; i < m_size; i++)
	{
		m_masterIndexBuffer.push_back(i);
	}
}

void PainterlyParticleSystem::initializeArray(std::string objectfilepath) {
	objl::Loader loader;
	loader.LoadFile(objectfilepath);
	std::cout << loader.LoadedMeshes[0].MeshName << std::endl;
	std::vector<objl::Vertex> vertices = loader.LoadedMeshes[0].Vertices;
	std::vector<unsigned int> indices = loader.LoadedMeshes[0].Indices;
	std::cout << indices.size() << std::endl;
	std::cout << vertices.size() << std::endl;

	float minArea;
	//loop through mesh triangles to get area data
	for (int i = 0; i < indices.size(); i += 3) {
		if (i == 0) {
			minArea = getTriangleArea(glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
									  glm::vec3(vertices[i+1].Position.X, vertices[i+1].Position.Y, vertices[i+1].Position.Z),
									  glm::vec3(vertices[i+2].Position.X, vertices[i+2].Position.Y, vertices[i+2].Position.Z));
		}
		else {
			float area = getTriangleArea(glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
				glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
				glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z));
			if (area < minArea) {
				minArea = area;
			}
		}	
	}
	std::cout << "Min area calculated: " << std::endl;
	std::cout << minArea << std::endl;
	//loop through indices again and place particles
	for (int i = 0; i < indices.size(); i += 3) {
		PaintParticle center;
		center.position.w = 1.0f;
		center.position.x = (vertices[i].Position.X + vertices[i+1].Position.X + vertices[i+2].Position.X) / 3.0f;
		center.position.y = (vertices[i].Position.Y + vertices[i+1].Position.Y + vertices[i+2].Position.Y) / 3.0f;
		center.position.z = (vertices[i].Position.Z + vertices[i+1].Position.Z + vertices[i+2].Position.Z) / 3.0f;
		m_particles.push_back(center);
	}
	m_size = m_particles.size();
	std::cout << m_size << std::endl;
}

unsigned int PainterlyParticleSystem::getShaderID()
/*
returns the program ID of the vertex/fragment shader program associated with this particle system
]*/
{
	return shader.getID();
}

/*
sets model related shader uniforms and makes draw call
*/
void PainterlyParticleSystem::batchRenderSystem()
{
	shader.bind();
	IndexBuffer ib(m_masterIndexBuffer.data(), m_masterIndexBuffer.size());
	shader.setUniformMat4f("u_model", m_transformationMatrix);
	renderer.draw(va, ib, shader);
}

void PainterlyParticleSystem::onUpdate() {
	batchRenderSystem();
}

void PainterlyParticleSystem::setTransformationMatrix(glm::mat4 tMat)
{
	m_transformationMatrix = tMat;
}

Shader& PainterlyParticleSystem::getShader()
{
	return shader;
}

float PainterlyParticleSystem::getTriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = a - c;
	return glm::length(glm::cross(ab, ac)) / 2;
}