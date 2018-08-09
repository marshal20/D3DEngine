#include "states.h"

#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "checks.h"

#include <d3d11.h>


struct BlendState::BlendStateBuffers
{
	InterPtr<ID3D11BlendState> pBlendState;
};

BlendState::BlendState()
{
	m_buffers = std::make_unique<BlendStateBuffers>();
}

BlendState::~BlendState()
{

}

void BlendState::init()
{
	D3D11_BLEND_DESC blendDesc;
	HRESULT hr;

	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = DeviceHandle::pDevice->CreateBlendState(&blendDesc, &m_buffers->pBlendState);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateBlendState(...) failed.");
}

void BlendState::cleanup()
{

}

void BlendState::use()
{
	float blendFactor[4];

	blendFactor[0] = 1.0f;
	blendFactor[1] = 1.0f;
	blendFactor[2] = 1.0f;
	blendFactor[3] = 1.0f;

	DeviceHandle::pContext->OMSetBlendState(m_buffers->pBlendState, blendFactor, 0xFFFFFFFF);
}
