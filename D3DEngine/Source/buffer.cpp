#include "buffer.h"

#include <d3d11.h>

#include "pointerutil.h"
#include "renderdevicehandle.h"
#include "checks.h"


struct Buffer::BufferBuffers
{
	InterPtr<ID3D11Buffer> pBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
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
	D3D11_SUBRESOURCE_DATA* pSubsourceData;
	D3D11_USAGE usage;
	UINT bindFlags;
	UINT cpuaccessflag;

	m_type = type;
	m_map = map;
	m_size = size;

	bindFlags = D3D11_BIND_VERTEX_BUFFER;
	switch (m_type)
	{
	case Type::Vertex:
		bindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case Type::Index:
		bindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	case Type::Constant:
		bindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;
	default:
		break;
	}

	cpuaccessflag = D3D11_CPU_ACCESS_WRITE;
	usage = D3D11_USAGE_DYNAMIC;
	switch (m_map)
	{
	case Map::Write:
		cpuaccessflag = D3D11_CPU_ACCESS_WRITE;
		usage = D3D11_USAGE_DYNAMIC;
		break;
	case Map::None:
		cpuaccessflag = 0;
		usage = D3D11_USAGE_DEFAULT;
		break;
	default:
		break;
	}

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = usage;
	bufferDesc.ByteWidth = m_size;
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.CPUAccessFlags = cpuaccessflag;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	subsourceData.pSysMem = pData;
	subsourceData.SysMemPitch = 0;
	subsourceData.SysMemSlicePitch = 0;

	pSubsourceData = pData == nullptr ? nullptr : &subsourceData;

	D3D11CALL(DeviceHandle::pDevice->CreateBuffer(&bufferDesc, pSubsourceData, &m_buffers->pBuffer));

	ZeroMemory(&m_buffers->mappedSubresource, sizeof(m_buffers->mappedSubresource));
}

void Buffer::cleanup()
{

}

void* Buffer::map()
{
	
	D3D11CALL(DeviceHandle::pContext->Map(m_buffers->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_buffers->mappedSubresource));

	return m_buffers->mappedSubresource.pData;
}

void Buffer::unmap()
{
	DeviceHandle::pContext->Unmap(m_buffers->pBuffer, 0);
}

void Buffer::updateData(void* data)
{
	if (m_map == Map::None)
	{
		DeviceHandle::pContext->UpdateSubresource(m_buffers->pBuffer, 0, nullptr, data, 0, 0);
	}
	else
	{
		void* mappedBuffer;

		mappedBuffer = map();
		memcpy(mappedBuffer, data, m_size);
		unmap();
	}
}

void* Buffer::getInternal()
{
	return m_buffers->pBuffer;
}
