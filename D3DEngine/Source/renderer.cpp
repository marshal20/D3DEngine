#include "renderer.h"
#include <d3d11.h>
#include "checks.h"
#include "renderdeviceImpl.h"

struct Renderer::RendererBuffers
{
	InterPtr<ID3D11Buffer> pVertexBuffer;
	InterPtr<ID3D11Buffer> pIndexBuffer;
};

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::init(RenderDevice* targetdevice)
{
	m_devicehandle.copy(targetdevice->getImplementation());

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexSubsourceData, indexSubsourceData;
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

	// vertexbuffer creating
	D3D11CALL(m_devicehandle->pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubsourceData, &m_buffers->pVertexBuffer));

	// index buffer
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// index subsource
	indexSubsourceData.pSysMem = indexData;
	indexSubsourceData.SysMemPitch = 0;
	indexSubsourceData.SysMemSlicePitch = 0;

	// index buffer creating
	D3D11CALL(m_devicehandle->pDevice->CreateBuffer(&indexBufferDesc, &indexSubsourceData, &m_buffers->pIndexBuffer));

	// free out buffers
	delete[] vertexData;
	delete[] indexData;

	// RETHINK


	// END RETHINK

	m_shader.init(targetdevice, "Resources/Shaders/simple.vs",
								 "Resources/Shaders/simple.ps");
}

void Renderer::cleanup()
{
	m_shader.cleanup();
}

void Renderer::render()
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	m_devicehandle->pContext->IASetVertexBuffers(0, 1, &m_buffers->pVertexBuffer, &stride, &offset);

	m_devicehandle->pContext->IASetIndexBuffer(m_buffers->pIndexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

	m_devicehandle->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader.render(6);


	return;
}