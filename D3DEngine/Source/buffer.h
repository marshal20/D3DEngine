#pragma once
#include <memory>
#include "renderdevice.h"

class Buffer
{
public:
	enum class Type { Vertex, Index };

public:
	Buffer();
	~Buffer();

	void init(const size_t size, const char* pData, Type type);
	void cleanup();

private:
	friend class Renderer;
	void* getInternal();

private:
	struct BufferBuffers;
	std::unique_ptr<BufferBuffers> m_buffers;
	Type m_type;
};