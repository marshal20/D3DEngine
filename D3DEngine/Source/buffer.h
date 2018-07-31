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

	void init(RenderDevice& renderdevice, const size_t size, const char* pData, Type type);
	void cleanup();

private:
	friend class Renderer;
	void* getInternal();

private:
	std::shared_ptr<RenderDeviceImpl> m_devicehandle;
	struct BufferBuffers;
	std::unique_ptr<BufferBuffers> m_buffers;
	Type m_type;
};