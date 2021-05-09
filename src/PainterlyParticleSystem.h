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

struct Particle {
	glm::vec4 position;
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
	public:
		PainterlyParticleSystem(int size, std::string vfshaderfilepath, std::string computefilepath);
};

