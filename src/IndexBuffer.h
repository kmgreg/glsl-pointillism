/**
class to represent an index buffer. makes the render loop much neater. adapted from youtube tutorials on openGL

**/

#pragma once
class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count; //number of indices

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();
	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_count; }
};

