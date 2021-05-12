/**
@author = Lauren Cole, Kurt Gregorek
Class to represent a particle system for the static painterly shader particles

**/


#include "PainterlyParticleSystem.h"
#include "OBJ_Loader.h"
PainterlyParticleSystem::PainterlyParticleSystem(int size, std::string vfshaderfilepath, std::string objectfilepath, std::string geoshaderfilepath) :
	vb(), shader(vfshaderfilepath, geoshaderfilepath), m_size(size), m_transformationMatrix(1.0f), minAreaLeeway(2.0f)
{
	//first we are going to make the vertex object array
	layout.push<float>(4); //push position
	initializeArray(objectfilepath);
	generateMasterIndexArray();
	vb.init(m_particles.data(), m_size * sizeof(PaintParticle), DYNAMIC_DRAW);
	va.addBuffer(vb, layout);
	m_objColor = glm::vec4(0.1, 0.7, 1.0, 1.0);
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

		//we pushed the center point in the triangle
		//if our triangle is too big we need to add more points for coverage
		//inner loop to add extra points
		float area = getTriangleArea(
			glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
			glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
			glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z));
		if (area > minArea*minAreaLeeway) {
			std::vector<PaintParticle> additionalPoints = addPoints(minArea*minAreaLeeway,
				glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
				glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
				glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z)
			);
			m_particles.insert(m_particles.end(), additionalPoints.begin(), additionalPoints.end());
		}
	}
	m_size = m_particles.size();
	std::cout << m_size << std::endl;
}

std::vector<PaintParticle> PainterlyParticleSystem::addPoints(float minArea, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	//recursion in c++
	//im very sad i have to do this :(
	//anyway heres the base case
	float area = getTriangleArea(a, b, c);
	std::vector<PaintParticle> points;
	if (area <= minArea) {
		points.push_back(getTriangleCenterAsPaintParticle(a, b, c));
		return points;
	}
	//if the triangle is big we gotta break it into littler ones
	else {
		//still add our center point
		points.push_back(getTriangleCenterAsPaintParticle(a, b, c));
		glm::vec3 center = glm::vec3(
			(a.x + b.x + c.x) / 3.0f,
			(a.y + b.y + c.y) / 3.0f,
			(a.z + b.z + c.z) / 3.0f
		);
		std::vector<PaintParticle> pointsAB = addPoints(minArea, a, b, center);
		std::vector<PaintParticle> pointsAC = addPoints(minArea, a, c, center);
		std::vector<PaintParticle> pointsBC = addPoints(minArea, b, c, center);

		//add the points in our return vectors to our main points vector
		points.insert(points.end(), pointsAB.begin(), pointsAB.end());
		points.insert(points.end(), pointsAC.begin(), pointsAC.end());
		points.insert(points.end(), pointsBC.begin(), pointsBC.end());

		return points;
	}
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
	shader.setUniform4fv("objColor", m_objColor);
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

PaintParticle PainterlyParticleSystem::getTriangleCenterAsPaintParticle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	PaintParticle center;
	center.position.w = 1.0f;
	center.position.x = (a.x + b.x + c.x) / 3.0f;
	center.position.y = (a.y + b.y + c.y) / 3.0f;
	center.position.z = (a.z + b.z + c.z) / 3.0f;
	return center;
}

void PainterlyParticleSystem::setMinAreaLeeway(float leeway)
{
	minAreaLeeway = leeway;
}

void PainterlyParticleSystem::setObjColor(glm::vec4 color)
{
	m_objColor = color;
}

glm::vec4 PainterlyParticleSystem::getObjColor()
{
	return m_objColor;
}