#include "renderer.h"

#include <d3d11.h>

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "buffer.h"


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

void Renderer::render(const Model& model, const DirectX::XMFLOAT4& Brightness)
{
	m_shader->setConstantBufferValue((char*)&Brightness, sizeof(DirectX::XMFLOAT4));

	model.bind();
	DeviceHandle::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_shader->render(model.getIndexCount());
}