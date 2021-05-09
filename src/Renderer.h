#pragma once
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
//TODO - rendering modes - triangles etc
/*
@contributor = Lauren Cole
*/

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	int renderingMode;

	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void drawRange(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int offset); //offset in bytes!
	void enableBlend();
	void setRenderingMode();
};