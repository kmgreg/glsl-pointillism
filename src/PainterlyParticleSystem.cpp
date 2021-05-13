/**
@author = Lauren Cole, Kurt Gregorek
Class to represent a particle system for the static painterly shader particles

**/


#include "PainterlyParticleSystem.h"
#include "OBJ_Loader.h"
PainterlyParticleSystem::PainterlyParticleSystem(std::string vfshaderfilepath, std::string objectfilepath, std::string geoshaderfilepath, float leeway) :
	vb(), shader(vfshaderfilepath, geoshaderfilepath), m_size(0), m_transformationMatrix(1.0f), minAreaLeeway(leeway), pointDistance(0.01f)
{
	//first we are going to make the vertex object array
	layout.push<float>(4); //push position
	layout.push<float>(3); // Normals
	//initialize buffers
	initializeArray(objectfilepath);
	generateMasterIndexArray();
	vb.init(m_particles.data(), m_size * sizeof(PaintParticle), DYNAMIC_DRAW);
	va.addBuffer(vb, layout);
	m_objColor = glm::vec4(0.1, 0.7, 1.0, 1.0);
}

PainterlyParticleSystem::PainterlyParticleSystem(std::string objectfilepath, Shader& precompiledshader, float leeway) : 
	m_transformationMatrix(1.0f), m_size(0), minAreaLeeway(leeway), pointDistance(0.01f)
{
	layout.push<float>(4); //push position
	layout.push<float>(3); // Normal
	initializeArray(objectfilepath);
	generateMasterIndexArray();
	vb.init(m_particles.data(), m_size * sizeof(PaintParticle), DYNAMIC_DRAW);
	va.addBuffer(vb, layout);
	m_objColor = glm::vec4(0.1, 0.7, 1.0, 1.0);
	shader = precompiledshader;

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
	std::cout << "indices size, vertices size: " << std::endl;
	std::cout << indices.size() << std::endl;
	std::cout << vertices.size() << std::endl;

	float minArea(getTriangleArea(glm::vec3(vertices[0].Position.X, vertices[0].Position.Y, vertices[0].Position.Z),
		glm::vec3(vertices[0 + 1].Position.X, vertices[0 + 1].Position.Y, vertices[0 + 1].Position.Z),
		glm::vec3(vertices[0 + 2].Position.X, vertices[0 + 2].Position.Y, vertices[0 + 2].Position.Z)));{}
	std::cout << minArea << std::endl;
	//loop through mesh triangles to get area data
	for (int i = 0; i < indices.size(); i += 3) {
		float area = getTriangleArea(glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
			glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
			glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z));
		if (area < minArea) {
			minArea = area;
		}
	}
	std::cout << "Min area calculated: " << std::endl;
	std::cout << minArea << std::endl;

	//loop through indices again and place particles
	for (int i = 0; i < indices.size(); i += 3) {
		PaintParticle center;
		center.position.w = 1.0f;
		center.position.x = (vertices[i].Position.X + vertices[i+1].Position.X + vertices[i+2].Position.X) / 3.0f;
		center.position.y = (vertices[i].Position.Y + vertices[i + 1].Position.Y + vertices[i + 2].Position.Y) / 3.0f;
		center.position.z = (vertices[i].Position.Z + vertices[i + 1].Position.Z + vertices[i + 2].Position.Z) / 3.0f;
		center.normal.x = (vertices[i].Normal.X + vertices[i + 1].Normal.X + vertices[i + 2].Normal.X) / 3.0f;
		center.normal.y = (vertices[i].Normal.Y + vertices[i + 1].Normal.Y + vertices[i + 2].Normal.Y) / 3.0f;
		center.normal.z = (vertices[i].Normal.Z + vertices[i + 1].Normal.Z + vertices[i + 2].Normal.Z) / 3.0f;
		m_particles.push_back(center);

		//we pushed the center point in the triangle
		//if our triangle is too big we need to add more points for coverage
		//inner loop to add extra points
		float area = getTriangleArea(
			glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
			glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
			glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z));
		if (area > minArea*minAreaLeeway) {
			std::vector<PaintParticle> additionalPoints = addPoints(minArea * minAreaLeeway,
				glm::vec3(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z),
				glm::vec3(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z),
				glm::vec3(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z),
				center.normal
			);
			m_particles.insert(m_particles.end(), additionalPoints.begin(), additionalPoints.end());
		}
	}
	m_size = m_particles.size();
	std::cout << m_size << std::endl;
}

std::vector<PaintParticle> PainterlyParticleSystem::addPoints(float minArea, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal)
{
	//recursion in c++
	//im very sad i have to do this :(
	//anyway heres the base case
	float area = getTriangleArea(a, b, c);
	std::vector<PaintParticle> points;
	if (area <= minArea) {
		points.push_back(getTriangleCenterAsPaintParticle(a, b, c, normal));
		return points;
	}
	//if the triangle is big we gotta break it into littler ones
	else {
		//still add our center point
		points.push_back(getTriangleCenterAsPaintParticle(a, b, c, normal));
		glm::vec3 center = glm::vec3(
			(a.x + b.x + c.x) / 3.0f,
			(a.y + b.y + c.y) / 3.0f,
			(a.z + b.z + c.z) / 3.0f
		);
		std::vector<PaintParticle> pointsAB = addPoints(minArea, a, b, center, normal);
		std::vector<PaintParticle> pointsAC = addPoints(minArea, a, c, center, normal);
		std::vector<PaintParticle> pointsBC = addPoints(minArea, b, c, center, normal);

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
	this->shader.setUniform4fv("diffuseColor", this->diff);
	this->shader.setUniform4fv("ambientColor", this->ambi);
	this->shader.setUniform4fv("specularColor", this->spec);
	this->shader.setUniform1f("specExp", this->specExp);
	this->shader.setUniform3fv("kCoeff", this->kCoeff);
/*
	shader.setUniform4fv("objColor", m_objColor);
	shader.setUniform1f("u_pointDistance", pointDistance);
*/
	renderer.draw(va, ib, shader);
}

void PainterlyParticleSystem::onUpdate() {
	batchRenderSystem();
}

void PainterlyParticleSystem::setTransformationMatrix(glm::mat4 tMat)
{
	m_transformationMatrix = tMat;
}

void PainterlyParticleSystem::setPhongVariables(glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular, GLfloat specExp, glm::vec3 kCoeff)
{
	this->diff = diffuse;
	this->ambi = ambient;
	this->spec = specular;
	this->specExp = specExp;
	this->kCoeff = kCoeff;

	this->shader.setUniform4fv("diffuseColor", this->diff);
	this->shader.setUniform4fv("ambientColor", this->ambi);
	this->shader.setUniform4fv("specularColor", this->spec);
	this->shader.setUniform1f("specExp", this->specExp);
	this->shader.setUniform3fv("kCoeff", this->kCoeff);
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

PaintParticle PainterlyParticleSystem::getTriangleCenterAsPaintParticle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal)
{
	PaintParticle center;
	center.position.w = 1.0f;
	center.position.x = (a.x + b.x + c.x) / 3.0f;
	center.position.y = (a.y + b.y + c.y) / 3.0f;
	center.position.z = (a.z + b.z + c.z) / 3.0f;
	center.normal = normal;
	return center;
}

void PainterlyParticleSystem::setObjColor(glm::vec4 color)
{
	m_objColor = color;
}

glm::vec4 PainterlyParticleSystem::getObjColor()
{
	return m_objColor;
}

void PainterlyParticleSystem::setPointDistance(float pd)
{
	pointDistance = pd;
}