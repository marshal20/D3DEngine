#include "renderer.h"

#include <d3d11.h>

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "buffer.h"


struct Renderer::RendererBuffers
{
	Buffer vertexBuffer;
	Buffer indexBuffer;
};

Renderer::Renderer()
{
	m_buffers = std::make_unique<RendererBuffers>();
}

Renderer::~Renderer()
{

}

void Renderer::init()
{
	VertexType* vertexData;
	unsigned long* indexData;
	unsigned int vertexCount, indexCount;
	float width;

	width = 0.5f;

	vertexCount = 4;
	indexCount = 6;

	vertexData = new VertexType[vertexCount];
	indexData = new unsigned long[indexCount];

	vertexData[0].pos = DirectX::XMFLOAT3(-width, -width, 0.0f);  // Bottom left.
	vertexData[1].pos = DirectX::XMFLOAT3(-width, +width, 0.0f);  // Top left.
	vertexData[2].pos = DirectX::XMFLOAT3(+width, +width, 0.0f);  // Top right.
	vertexData[3].pos = DirectX::XMFLOAT3(+width, -width, 0.0f);  // Bottom right.

	indexData[0] = 0;  // Bottom left.
	indexData[1] = 1;  // Top left.
	indexData[2] = 2;  // Top right.

	indexData[3] = 3;  // Bottom right.
	indexData[4] = 0;  // Bottom left.
	indexData[5] = 2;  // Top right.

	m_buffers->vertexBuffer.init(sizeof(VertexType) * 4, (char*)vertexData, Buffer::Type::Vertex);
	m_buffers->indexBuffer.init(sizeof(unsigned long) * 6, (char*)indexData, Buffer::Type::Index);

	// free out buffers
	delete[] vertexData;
	delete[] indexData;

	m_shader = ShaderFactory::getShader("Simple");
}

void Renderer::cleanup()
{
	
}

void Renderer::render()
{
	ID3D11Buffer* pVertexBuffer;
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(VertexType);
	offset = 0;

	pVertexBuffer = (ID3D11Buffer*)m_buffers->vertexBuffer.getInternal();
	DeviceHandle::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	DeviceHandle::pContext->IASetIndexBuffer((ID3D11Buffer*)m_buffers->indexBuffer.getInternal(), DXGI_FORMAT_R32_UINT, 0);

	DeviceHandle::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader->render(6);


	return;
}