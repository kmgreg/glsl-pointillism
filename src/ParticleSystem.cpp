#include "ParticleSystem.h"
#include <iostream>
/*
@author = Lauren Cole
Class to represent a particle system

*/

ParticleSystem::ParticleSystem(int size, glm::vec3 origin, std::string vffilepath, std::string computefilepath) 
	: vb(), shader(vffilepath), m_size(size), m_head(size - 1), m_tail(size-1), m_maxAge(1.0f), m_numAlive(0), random(), cShader(computefilepath), texture("res/textures/noiseTexture.png")
{
	//CONSTRUCT VAO LAYOUT
	layout.push<float>(4); //push position
	layout.push<float>(4); //push velocity
	layout.push<float>(4); //push particle age (+ acceleration)
	layout.push<float>(4); //push particle color

	//initialize other variables
	texture.bind(0);
	m_systemOrigin = origin;
	initializeArray();
	generateMasterIndexArray();
	vb.init(m_particles.data(), m_size * sizeof(Particle), DYNAMIC_DRAW);
	va.addBuffer(vb, layout);
}

/*
index particle array
*/
void ParticleSystem::initializeArray()
{
	for (int i = 0; i < m_size; i++)
	{
		m_particles.push_back(createParticle());
	}
	std::cout << m_particles.size() << std::endl;
}

/*
generates index buffer for the system
*/
void ParticleSystem::generateMasterIndexArray()
{
	for (int i = 0; i < m_size; i++)
	{
		m_masterIndexBuffer.push_back(i);
	}
}


/*
returns id of vertex fragment shader program
*/
unsigned int ParticleSystem::getShaderID()
/*
returns the program ID of the vertex/fragment shader program associated with this particle system
]*/
{
	return shader.getID();
}

void ParticleSystem::updateOrigin(float xpos, float ypos)
/*
updates the particle spawn point. expects that xpos and ypos are already converted from screen to world coordinates
*/
{
	m_systemOrigin[0] = xpos;
	m_systemOrigin[1] = ypos;
}


/*
sets vf shader uniforms and makes draw call
*/
void ParticleSystem::batchRenderSystem()
{
	shader.bind();
	//buildVBO();
	//shader.setUniform1i("u_text", 0);
	vb.updateBufferData(m_masterVBO.data(), m_masterVBO.size() * sizeof(Particle), 0, DYNAMIC_DRAW);
	IndexBuffer ib(m_masterIndexBuffer.data(), m_masterVBO.size());
	glm::mat4 model = glm::mat4(1.0);
	shader.setUniformMat4f("u_model", model);
	renderer.draw(va, ib, shader);
}

void ParticleSystem::emit()
/*
emits one particle from the particle system by setting it to active.
*/
{
	if (m_particles[m_head].age[0] <= 0.00f)
	{
		resetParticle(m_head);
		m_numAlive++;
		if (m_head == 0) m_head = m_size - 1;
		else m_head--;
	}
}

void ParticleSystem::emit(int p)
/*
emits p particles 
*/
{
	for (int i = 0; i < p; i++)
	{
		emit();
	}
}

/*
TODO: make this parameterizable, possibly in the controller class
returns random velocity values to be used when resetting a particle
*/
glm::vec3 ParticleSystem::randomizeParticleVelocity()
{
	//SMOKE RANDOMIZATION
	float x = random.getFloatInRange(-0.01, 0.01);
	float y = random.getFloatInRange(0.01, 0.02);
	float z = random.getFloatInRange(-0.01, 0.01);

	//RAINBOW RANDOMIZATION
	//float x = random.getFloatInRange(-0.001, 0.001);
	//float y = random.getFloatInRange(-0.001, 0.001);
	//float z = random.getFloatInRange(-0.001, 0.001);
	return glm::vec3(x, y, z);
}

/*
build vertex buffer object for drawing by only pushing living particles
*/
void ParticleSystem::buildVBO()
{
	m_masterVBO.clear();
	for (int i = 0; i < m_size; i++)
	{
		if (m_particles[i].age[0] > 0.0f)
		{
			m_masterVBO.push_back(m_particles[i]);
		}
	}
}

/*
perform each render loop. dispatches compute shader to update system and reads buffer info back from GPU
*/
void ParticleSystem::onUpdate()
{
	/*CODE TO TRY TO RENDER VECTOR OF STRUCT ARRAY*/

	ShaderStorageBufferObject ssbo = ShaderStorageBufferObject(0, m_particles.data(), sizeof(Particle) * m_size);
	/*GLuint ssboID;
	glGenBuffers(1, &ssboID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vertex) * m_size * 4, m_particles.data(), GL_DYNAMIC_DRAW);*/
	
	//group size height = 8, width =8
	
	cShader.use();
	cShader.setUniform3fv("u_lerpColors", 6, lerpColors);
	cShader.setUniform1i("u_texture", 0);
	cShader.dispatch((GLuint)m_size / 4, 1, 1);

	GLCall(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	GLint bufMask = GL_MAP_READ_BIT;
	m_masterVBO.clear();
	 GLfloat* ptr= ssbo.mapBuffer(m_size *sizeof(Particle), bufMask);
	//std::cout << ptr << std::endl;
	//GLCall(glGetNamedBufferSubData(ssboID, 0, m_particles.size() * 4 * sizeof(Vertex), test.data()));
	// write the data back to cpu storage
	int particleIndex = 0;
	for (int i = 0; i < m_size * 16; i+=16) 
	{
		m_particles[particleIndex].positions = glm::vec4(ptr[i], ptr[i + 1], ptr[i + 2], ptr[i + 3]);
		m_particles[particleIndex].velocity = glm::vec4(ptr[i + 4], ptr[i + 5], ptr[i + 6], ptr[i + 7]);
		m_particles[particleIndex].age = glm::vec4(ptr[i + 8], ptr[i + 9], ptr[i + 10], ptr[i + 11]);
		m_particles[particleIndex].color = glm::vec4(ptr[i + 12], ptr[i + 13], ptr[i + 14], ptr[i + 15]);

		if (m_particles[particleIndex].age[0] > 0.0f)
		{
			m_masterVBO.push_back(m_particles[particleIndex]);
		}

		//incrememnt particle index
		particleIndex += 1;
	}

	ssbo.unmapBuffer();
	batchRenderSystem();
}

/*
create a new particle
*/
Particle ParticleSystem::createParticle()
{
	glm::vec4 velocity = glm::vec4(randomizeParticleVelocity(), 1.0f);
	Particle v1 = {glm::vec4(m_systemOrigin, 1.0f), velocity, glm::vec4(0.0f), glm::vec4(0.0f)};
	return v1;
}

/*
reset particle located at index i in m_particles, give new random velocity values
*/
void ParticleSystem::resetParticle(int i)
{
	glm::vec3 spawn = m_systemOrigin;
	glm::vec4 velocity = glm::vec4(randomizeParticleVelocity(), 1.0f);
	m_particles[i] = {glm::vec4(spawn, 1.0f), velocity, glm::vec4(1.0f, random.getRandFloat(), 1.0f, 1.0f), glm::vec4(0.0f)};
}