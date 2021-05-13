#include "IndexBuffer.h"
#include "Renderer.h"
/*
@contributor = Lauren Cole
based off of Cherno tutorials on https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw
functionality is extended by contributor
*/

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count)
{
   
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
 /*   std::cout << "ib id: " << m_rendererID << std::endl;
    std::cout << data[0] << " " << data[1] <<  " " << data[2] << " " << data[3] << " " << data[4] << " " << data[5] << std::endl;*/
    //size is in bytes
}
IndexBuffer::IndexBuffer() : m_count(2)
{
    
    unsigned int data[] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int c = 6;

    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, c * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}