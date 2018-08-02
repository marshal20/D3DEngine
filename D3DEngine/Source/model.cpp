#include "model.h"

#include <d3d11.h>

#include "buffer.h"
#include "renderdevicehandle.h"


struct Model::ModelBuffers
{
	Buffer vertexBuffer;
	Buffer indexBuffer;
};


Model::Model()
{
	m_buffers = std::make_unique<ModelBuffers>();
}

Model::~Model()
{

}

void Model::init(Texture* texture)
{
	VertexType* vertexData;
	unsigned long* indexData;
	unsigned int vertexCount;

	vertexCount = 4;
	m_indexCount = 6;
	m_texture = texture;

	vertexData = new VertexType[vertexCount];
	indexData = new unsigned long[m_indexCount];

	vertexData[0].position = DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);  // Bottom left.
	vertexData[0].color = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertexData[0].coord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertexData[1].position = DirectX::XMFLOAT4(-1.0f, +1.0f, 0.0f, 1.0f);  // Top left.
	vertexData[1].color = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexData[1].coord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertexData[2].position = DirectX::XMFLOAT4(+1.0f, +1.0f, 0.0f, 1.0f);  // Top right.
	vertexData[2].color = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertexData[2].coord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertexData[3].position = DirectX::XMFLOAT4(+1.0f, -1.0f, 0.0f, 1.0f);  // Bottom right.
	vertexData[3].color = DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f);
	vertexData[3].coord = DirectX::XMFLOAT2(1.0f, 1.0f);

	indexData[0] = 0;  // Bottom left.
	indexData[1] = 1;  // Top left.
	indexData[2] = 2;  // Top right.

	indexData[3] = 3;  // Bottom right.
	indexData[4] = 0;  // Bottom left.
	indexData[5] = 2;  // Top right.

	m_buffers->vertexBuffer.init(sizeof(VertexType) * vertexCount, (char*)vertexData, Buffer::Type::Vertex);
	m_buffers->indexBuffer.init(sizeof(unsigned long) * m_indexCount, (char*)indexData, Buffer::Type::Index);

	// free out buffers
	delete[] vertexData;
	delete[] indexData;
}

void Model::cleanup()
{
	m_buffers->vertexBuffer.cleanup();
	m_buffers->indexBuffer.cleanup();
}

void Model::bind() const
{
	ID3D11Buffer* pVertexBuffer;
	ID3D11ShaderResourceView* pTex;
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(VertexType);
	offset = 0;

	pVertexBuffer = (ID3D11Buffer*)m_buffers->vertexBuffer.getInternal();
	DeviceHandle::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	DeviceHandle::pContext->IASetIndexBuffer((ID3D11Buffer*)m_buffers->indexBuffer.getInternal(), DXGI_FORMAT_R32_UINT, 0);

	pTex = (ID3D11ShaderResourceView*)m_texture->getView();

	DeviceHandle::pContext->PSSetShaderResources(0, 1, &pTex);
}

int Model::getIndexCount() const
{
	return m_indexCount;
}
