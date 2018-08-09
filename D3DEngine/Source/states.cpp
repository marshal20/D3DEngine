#include "states.h"

#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "checks.h"

#include <d3d11.h>

// BlendState

BlendState::BlendState()
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

	hr = DeviceHandle::pDevice->CreateBlendState(&blendDesc, &m_pBlendState);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateBlendState(...) failed.");
}

void BlendState::use()
{
	float blendFactor[4];

	blendFactor[0] = 1.0f;
	blendFactor[1] = 1.0f;
	blendFactor[2] = 1.0f;
	blendFactor[3] = 1.0f;

	DeviceHandle::pContext->OMSetBlendState(m_pBlendState, blendFactor, 0xFFFFFFFF);
}

// DepthStencilState

DepthStencilState::DepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT hr;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = DeviceHandle::pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateDepthStencilState(...) failed.");
}

void DepthStencilState::use()
{
	DeviceHandle::pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}
