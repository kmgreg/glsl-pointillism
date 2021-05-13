/*
@author = Lauren Cole
class to represent a 3d animated particle system
uses compute shaders to calculate animation
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

/**
class to represent a particle system contains a struct to represent a particle
@author = Lauren Cole
**/

//TODO:
		//optimize backwards emit - but it is working for now!
		//add particle system controller class to handle params + buttons?
		// apply wind

#pragma once

struct Particle
{
	glm::vec4 positions;
	glm::vec4 velocity;
	glm::vec4 age;
	glm::vec4 color;
};



class ParticleSystem
{
private:
	int m_size; //size of the particle system. make parameterizable in the future
	float m_maxAge; //age to give the particles upon initialization
	glm::vec3 m_systemOrigin;
	int m_head; // keep track of the index at which we are spawning particles
	int m_tail; // keep track of index at which particles are fading
	glm::vec4 m_spawnPoint; //origin of the particle system
	int m_numAlive;
	std::vector<float> pAges;
	
	

	std::vector<Particle> m_particles;
	glm::vec3 particleScale = glm::vec3(0.01f, 0.01, 0.0f);
	std::vector<unsigned int> m_masterIndexBuffer;
	std::vector<Particle> m_masterVBO;
	void resetParticle(int i);

	
	/* PARTICLE DATA INFORMATION. all our particles are tiny quads being moved around by other calculations. this is fine*/
	std::vector<unsigned int> indices;
	glm::vec3 lerpColors[6] = {
	glm::vec3(1.0f, 0.059f, 0.18f),
	glm::vec3(1.0f, 0.443f, 0.09f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(0.1f, 1.0f, 0.1f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.627f, 0.141f, 0.898f)
	};


	/* initialize these on particle system construction. variables for open gl rendering*/
	VertexArray va;
	VertexBuffer vb; 
	IndexBuffer ib;
	VertexBufferLayout layout;
	Shader shader;
	ComputeShader cShader;
	//ShaderStorageBufferObject ssbo;
	Renderer renderer;
	Random random;
	void generateMasterIndexArray();
	glm::vec3 randomizeParticleVelocity();
	void updateParticles();
	void buildVBO();
	void initializeAgeVector();
	Texture texture;

public:
	ParticleSystem(int size, glm::vec3 origin, std::string vfshaderfilepath, std::string computefilepath);
	unsigned int getShaderID();
	void initializeArray();
	void updateOrigin(float xpos, float ypos); // function to update the origin of the particle system
	void onUpdate();
	Particle createParticle();
	void batchRenderSystem();
	void emit();
	void emit(int p);
	
};

