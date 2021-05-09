#pragma once
#define STATIC_DRAW 0
#define DYNAMIC_DRAW 1
/*
@contirbutor Lauren Cole - class to represent vertex buffer obejcts

*/
class VertexBuffer
{
private:
	unsigned int m_rendererID;

public:
	VertexBuffer(const void* data, unsigned int size, int usage);
	VertexBuffer();
	void init(const void* data, unsigned int size, int usage);
	~VertexBuffer();
	void bind() const;
	void unbind() const;
	void updateBufferData(const void* data, unsigned int size, unsigned int offset, int usage);
	unsigned int getID();
};

