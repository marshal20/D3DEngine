#pragma once
#include <memory>
#include "renderdevice.h"

struct ID3D11Buffer;

class Buffer
{
public:
	enum class Type { Vertex, Index, Constant };
	enum class Map { Write, None};

public:
	Buffer(const size_t size, const char* pData, Type type, Map map = Map::None);
	Buffer(const Buffer& other);
	~Buffer();

	void* map();
	void unmap();

	void updateData(void* data);

private:
	friend class Renderer;
	friend class Shader;
	friend class Model;
	void* getInternal();

private:
	InterPtr<ID3D11Buffer> m_pBuffer;
	Type m_type;
	Map m_map;
	size_t m_size;
};