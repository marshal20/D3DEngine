#include <CoreEngine\state\depthstencilstate.h>

#include <d3d11.h>

#include "internalstate.h"
#include "../utils/safemem.h"
#include "../utils/callcheck.h"

namespace ce
{
	DepthStencilState::DepthStencilState()
	{

	}

	DepthStencilState::~DepthStencilState()
	{

	}

	void DepthStencilState::init()
	{
		HRESULT hr;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		SAFE_RELEASE(m_depth_stencil_state);

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = m_depth_enable;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = m_stencil_enable;
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
		
		hr = ce::state::d3d::m_device->CreateDepthStencilState(&depthStencilDesc, &m_depth_stencil_state);
		CHECK_HR(hr);
	}

	void DepthStencilState::cleanup()
	{
		SAFE_RELEASE(m_depth_stencil_state);
	}

	void DepthStencilState::set_main()
	{
		ce::state::d3d::m_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
	}

	void DepthStencilState::set_depth_enable(bool enable)
	{
		m_depth_enable = enable;
	}

	void DepthStencilState::set_stencil_enable(bool enable)
	{
		m_stencil_enable = enable;
	}
}