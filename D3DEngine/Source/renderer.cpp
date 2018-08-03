#include "renderer.h"

#include <d3d11.h>

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "buffer.h"


Renderer::MatrixBuffer calcMatrixBuffer(const Camera& camera, const DirectX::XMMATRIX& modelTransform);

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::init()
{
	m_shader = ShaderFactory::getShader("Simple");
	m_constantBuffer.init(sizeof(MatrixBuffer), nullptr, Buffer::Type::Constant);
}

void Renderer::cleanup()
{
	m_constantBuffer.cleanup();
}

void Renderer::render(const Model& model, const Camera& camera)
{
	MatrixBuffer constantBuffer;

	constantBuffer = calcMatrixBuffer(camera, model.getTransformMatrix());
	updateConstantBuffers(m_constantBuffer, &constantBuffer);

	m_shader->use();

	// TODO: create sampler state
	//DeviceHandle::pContext->PSSetSamplers(...);
	
	model.bind();
	DeviceHandle::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceHandle::pContext->DrawIndexed(model.getIndexCount(), 0, 0);
}

void Renderer::updateConstantBuffers(Buffer& constantBuffer, const Renderer::MatrixBuffer* value)
{
	ID3D11Buffer* pConstantBuffer;

	// updating data
	constantBuffer.updateData((void*)value);

	// using the buffer
	pConstantBuffer = (ID3D11Buffer*)constantBuffer.getInternal();
	DeviceHandle::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
}


// HELPER FUNCTIONS

Renderer::MatrixBuffer calcMatrixBuffer(const Camera& camera, const DirectX::XMMATRIX& modelTransform)
{
	Renderer::MatrixBuffer matrixBuffer;
	float fov;
	float aspectRatio;

	matrixBuffer.model = modelTransform;
	matrixBuffer.world = DirectX::XMMatrixIdentity();
	matrixBuffer.view = camera.getView();

	fov = camera.getFov();
	aspectRatio = DeviceHandle::pRenderDevice->getAspectRatio();

	matrixBuffer.projection= DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 1000.0f);

	// transpose all matrices
	matrixBuffer.model = DirectX::XMMatrixTranspose(matrixBuffer.model);
	matrixBuffer.world = DirectX::XMMatrixTranspose(matrixBuffer.world);
	matrixBuffer.view = DirectX::XMMatrixTranspose(matrixBuffer.view);
	matrixBuffer.projection = DirectX::XMMatrixTranspose(matrixBuffer.projection);

	return matrixBuffer;
}
