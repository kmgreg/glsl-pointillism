#pragma once
#include "Renderer.h"
/*
@contributor lauren cole = class to represent textures
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

