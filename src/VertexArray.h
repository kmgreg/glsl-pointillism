 #pragma once

#include "VertexBuffer.h"
/*
@ontirbutor Lauren Cole - class to represent vertex arrray obejcts
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

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


