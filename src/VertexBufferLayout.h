#pragma once
#include<vector>
#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/*
@author = Lauren Cole
class to represent vertex buffer obejcts. 
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
struct VertexBufferElement {

    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT :  return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;

public:
    VertexBufferLayout() 
        : m_stride(0) {};
    template<typename T> 
    void push(unsigned int count)
    {
        static_assert(false);
    }
    template<>
    void push<float>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, count, GL_FALSE});
        m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    }
    template<>
    void push<unsigned int>(unsigned int count)
    {
       m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
       m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;

    }
    template<>
    void push<unsigned char>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;

    }
    inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
    inline unsigned int getSTride() const { return m_stride; }
};

