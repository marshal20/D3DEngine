#include "buffer.h"

#include <d3d11.h>

#include "pointerutil.h"
#include "renderdeviceImpl.h"
#include "checks.h"


/*
// vertex buffer
ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
vertexBufferDesc.ByteWidth = sizeof(VertexType) * 4;
vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
vertexBufferDesc.CPUAccessFlags = 0;
vertexBufferDesc.MiscFlags = 0;
vertexBufferDesc.StructureByteStride = 0;

// vertex subsource
vertexSubsourceData.pSysMem = vertexData;
vertexSubsourceData.SysMemPitch = 0;
vertexSubsourceData.SysMemSlicePitch = 0;
*/

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

void Buffer::init(RenderDevice& renderdevice, const size_t size, const char* pData, Type type)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subsourceData;
	UINT bindFlags;

	m_devicehandle = renderdevice.getImplementation();
	m_type = type;

	bindFlags = D3D11_BIND_VERTEX_BUFFER;
	switch (m_type)
	{
	case Type::Vertex:
		bindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case Type::Index:
		bindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	}

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	subsourceData.pSysMem = pData;
	subsourceData.SysMemPitch = 0;
	subsourceData.SysMemSlicePitch = 0;

	D3D11CALL(m_devicehandle->pDevice->CreateBuffer(&bufferDesc, &subsourceData, &m_buffers->pBuffer));
}

void Buffer::cleanup()
{

}

void* Buffer::getInternal()
{
	return m_buffers->pBuffer;
}
