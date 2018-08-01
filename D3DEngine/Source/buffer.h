#pragma once
#include <memory>
#include "renderdevice.h"

class Buffer
{
public:
	enum class Type { Vertex, Index };
	enum class Map { Write, None};

public:
	Buffer();
	~Buffer();

	void init(const size_t size, const char* pData, Type type, Map map = Map::None);
	void cleanup();

private:
	friend class Renderer;
	friend class Model;
	void* getInternal();

private:
	struct BufferBuffers;
	std::unique_ptr<BufferBuffers> m_buffers;
	Type m_type;
	Map m_map;
};