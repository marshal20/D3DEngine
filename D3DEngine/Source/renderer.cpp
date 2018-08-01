#include "renderer.h"

#include <d3d11.h>

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "buffer.h"


Renderer::MatrixBuffer calcMatrixBuffer(const Camera& camera);

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::init()
{
	m_shader = ShaderFactory::getShader("Simple");
}

void Renderer::cleanup()
{
	
}

void Renderer::render(const Model& model, const Camera& camera)
{
	MatrixBuffer constantBuffer;

	constantBuffer = calcMatrixBuffer(camera);

	m_shader->setConstantBufferValue((char*)&constantBuffer, sizeof(MatrixBuffer));

	model.bind();
	DeviceHandle::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_shader->render(model.getIndexCount());
}


// HELPER FUNCTIONS

Renderer::MatrixBuffer calcMatrixBuffer(const Camera& camera)
{
	Renderer::MatrixBuffer matrixBuffer;
	float fov;
	float aspectRatio;

	matrixBuffer.world = DirectX::XMMatrixIdentity();
	matrixBuffer.view = camera.getView();

	fov = (45.0f) * 3.14f / 180.0f; // fov = 45 degree
	aspectRatio = DeviceHandle::pRenderDevice->getAspectRatio();

	matrixBuffer.projection= DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 1000.0f);

	// transpose all matrices
	matrixBuffer.world = DirectX::XMMatrixTranspose(matrixBuffer.world);
	matrixBuffer.view = DirectX::XMMatrixTranspose(matrixBuffer.view);
	matrixBuffer.projection = DirectX::XMMatrixTranspose(matrixBuffer.projection);

	return matrixBuffer;
}
