#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

/*
@author = Lauren Cole
a class to represent shader storage buffer object
*/

#define VERTEX 0
#define COLOR 1
/*
class to wrap a shader storage buffer object,
*/
class ShaderStorageBufferObject
{
	
private:
	unsigned int m_rendererID;
	int m_bindingID;
public:
	ShaderStorageBufferObject(int bindingID, const void* data, unsigned int size); //binding IDs must be managed outside the ssbo class
	~ShaderStorageBufferObject();
	int getID();
	int getbindingID();
	void bind();
	void unbind();
	GLfloat* mapBuffer(unsigned int size, GLint bufmask);
	GLfloat* mapBuffer();
	void unmapBuffer();
	void updateBufferData(const void* data, unsigned int size);
	void bindAsArray(int usage);
	void draw(int n);
};

