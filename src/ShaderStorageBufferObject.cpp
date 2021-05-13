#include "ShaderStorageBufferObject.h"
//TODO:do you want  diff storage buffers for each objct?
//NOTE: sizeof only works for statically sized c/c++ arrays
/*
@author = Lauren Cole
a class to represent shader storage buffer object
*/
ShaderStorageBufferObject::ShaderStorageBufferObject(int bindingID, const void* data, unsigned int size)
{
	m_bindingID = bindingID;
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_rendererID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingID, m_rendererID));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, (const void*) data, GL_STATIC_DRAW));
	
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

int ShaderStorageBufferObject::getID()
{
	return m_rendererID;
}

int ShaderStorageBufferObject::getbindingID()
{
	return m_bindingID;
}

void ShaderStorageBufferObject::bind()
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_rendererID));
}

void ShaderStorageBufferObject::unbind()
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

GLfloat* ShaderStorageBufferObject::mapBuffer(unsigned int size, GLint bufmask)
{
	bind();
	GLfloat* ptr;
	ptr = (GLfloat*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size ,bufmask);
	return ptr;
}

void ShaderStorageBufferObject::unmapBuffer()
{
	GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}

void ShaderStorageBufferObject::updateBufferData(const void* data, unsigned int size)
{
	bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, (void*) data);
}

void ShaderStorageBufferObject::bindAsArray(int usage)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	if (usage == VERTEX)
	{
		GLCall(glVertexPointer(4, GL_FLOAT, 0, (void*)0));
		GLCall(glEnableClientState(GL_VERTEX_ARRAY));
	}

	else if (usage == COLOR)
	{
		GLCall(glColorPointer(4, GL_FLOAT, 0, (void*)0));
		GLCall(glEnableClientState(GL_COLOR_ARRAY));
	}
}

void ShaderStorageBufferObject::draw(int n)
{
	glPointSize(3.);
	glDrawArrays(GL_POINTS, 0, n);
	glPointSize(1.);
}
