#pragma once
#include "Renderer.h"
/*
@author = Lauren Cole
class to represent textures
adapted from OpenGL series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
class Texture
{
private:
	unsigned int m_rendererID;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bpp;

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	int getSlot();
};

