#include "VertexBuffer.h"
#include "Renderer.h"
/*
@contirbutor Lauren Cole - class to represent vertex buffer obejcts
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
VertexBuffer::VertexBuffer(const void* data, unsigned int size, int usage)
{
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    if (usage == STATIC_DRAW)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }



    //std::cout << "vb data: " << data << std::endl;

    //size is in bytes
}
VertexBuffer::VertexBuffer()
{
    /*float data[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f, 
         0.5f, 0.5f, 
         -0.5f, 0.5f,
    };
    unsigned int size = (4 * sizeof(float));

    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, (const void*) data, GL_STATIC_DRAW)); */
}

unsigned int VertexBuffer::getID()
{
    return m_rendererID;
}

void VertexBuffer::init(const void* data, unsigned int size, int usage)
{
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    if (usage == STATIC_DRAW)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::updateBufferData(const void* data, unsigned int size, unsigned int offset, int usage)
{
    bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, (void*) data));
}
