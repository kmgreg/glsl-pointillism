#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>
#include "VertexBufferLayout.h"
/*
@ontirbutor Lauren Cole - class to represent vertex arrray obejcts
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererID));

}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	bind();
	vb.bind();
		const auto& elements = layout.getElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.getSTride(), (const void*) offset));
			offset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_rendererID));

}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

