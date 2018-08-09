#include "model.h"

#include <d3d11.h>

#include "buffer.h"
#include "renderdevicehandle.h"

Model::Model(const Mesh& mesh, Texture* texture) : 
	m_texture(texture)
{
	const Mesh::Vertex* vertexData;
	const unsigned long* indexData;

	m_indexCount = mesh.indexData.size();

	vertexData = &mesh.vertexData[0];
	indexData = &mesh.indexData[0];

	m_vertexBuffer = std::make_unique<Buffer>(sizeof(Mesh::Vertex) * mesh.vertexData.size(), (const char*)vertexData, Buffer::Type::Vertex);
	m_indexBuffer = std::make_unique<Buffer>(sizeof(unsigned long) * mesh.indexData.size(), (const char*)indexData, Buffer::Type::Index);
}

void Model::bind() const
{
	ID3D11Buffer* pVertexBuffer;
	ID3D11ShaderResourceView* pTex;
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(Mesh::Vertex);
	offset = 0;

	pVertexBuffer = (ID3D11Buffer*)m_vertexBuffer->getInternal();
	DeviceHandle::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	DeviceHandle::pContext->IASetIndexBuffer((ID3D11Buffer*)m_indexBuffer->getInternal(), DXGI_FORMAT_R32_UINT, 0);

	pTex = (ID3D11ShaderResourceView*)m_texture->getView();

	DeviceHandle::pContext->PSSetShaderResources(0, 1, &pTex);
}

Transform& Model::transform()
{
	return m_transform;
}

int Model::getIndexCount() const
{
	return m_indexCount;
}

DirectX::XMMATRIX Model::getTransformMatrix() const
{
	DirectX::XMMATRIX tranformMatrix;
	DirectX::XMMATRIX translationMatrix;
	DirectX::XMMATRIX rotationMatrix;
	DirectX::XMMATRIX scalingMatrix;

	tranformMatrix = DirectX::XMMatrixIdentity();

	translationMatrix = DirectX::XMMatrixTranslation(m_transform.position.x, m_transform.position.y, m_transform.position.z);
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_transform.rotation.x * 0.0174532925f, m_transform.rotation.y * 0.0174532925f, m_transform.rotation.z * 0.0174532925f);
	scalingMatrix = DirectX::XMMatrixScaling(m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);

	tranformMatrix = DirectX::XMMatrixMultiply(tranformMatrix, scalingMatrix);
	tranformMatrix = DirectX::XMMatrixMultiply(tranformMatrix, rotationMatrix);
	tranformMatrix = DirectX::XMMatrixMultiply(tranformMatrix, translationMatrix);

	return tranformMatrix;
}
