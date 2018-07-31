#include "buffer.h"

#include <d3d11.h>

#include "pointerutil.h"
#include "renderdevicehandle.h"
#include "checks.h"


struct Buffer::BufferBuffers
{
	InterPtr<ID3D11Buffer> pBuffer;
};

Buffer::Buffer()
{
	m_buffers = std::make_unique<BufferBuffers>();
}

Buffer::~Buffer()
{

}

void Buffer::init(const size_t size, const char* pData, Type type, Map map)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subsourceData;
	UINT bindFlags;
	UINT cpuaccessflag;

	m_type = type;
	m_map = map;

	bindFlags = D3D11_BIND_VERTEX_BUFFER;
	switch (m_type)
	{
	case Type::Vertex:
		bindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case Type::Index:
		bindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	default:
		break;
	}

	cpuaccessflag = D3D11_CPU_ACCESS_WRITE;
	switch (m_map)
	{
	case Map::Write:
		cpuaccessflag = D3D11_CPU_ACCESS_WRITE;
		break;
	case Map::None:
		cpuaccessflag = 0;
		break;
	default:
		break;
	}

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.CPUAccessFlags = cpuaccessflag;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	subsourceData.pSysMem = pData;
	subsourceData.SysMemPitch = 0;
	subsourceData.SysMemSlicePitch = 0;

	D3D11CALL(DeviceHandle::pDevice->CreateBuffer(&bufferDesc, &subsourceData, &m_buffers->pBuffer));
}

void Buffer::cleanup()
{

}

void* Buffer::getInternal()
{
	return m_buffers->pBuffer;
}
