#include "Texture.h"
#include "stb_image.h"
/*
@contributor lauren cole = class to represent textures
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
Texture::Texture(const std::string& path) : 
	m_rendererID(0), m_filepath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)

{	
	//pngs store 0,0 at top left, openGL has 0,0 at bottom left
	stbi_set_flip_vertically_on_load(1);

	//local buffer is going to store our pixel information
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glGenerateMipmap(GL_TEXTURE_2D);
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	if (m_localBuffer)
	{
		stbi_image_free(m_localBuffer);
	}

}

/**
delete textures off gpu when the object is destroyed (when the object goes out of scope)
*/
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture::bind(unsigned int slot /*= 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot))
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


