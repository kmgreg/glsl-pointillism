 #pragma once

#include "VertexBuffer.h"
/*
@ontirbutor Lauren Cole - class to represent vertex arrray obejcts

*/
class VertexBufferLayout;
	
class VertexArray
{
private:
	unsigned int m_rendererID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};


