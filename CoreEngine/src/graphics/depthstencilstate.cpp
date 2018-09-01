#include <CoreEngine/graphics/depthstencilstate.h>

#include <d3d11.h>

#include "state/internalstate.h"
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

	void DepthStencilState::init(bool depth_enable, bool stencil_enable)
	{
		HRESULT hr;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		SAFE_RELEASE(m_depth_stencil_state);

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = depth_enable;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = stencil_enable;
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
		
		hr = ce::state::kcontext->get_device()->CreateDepthStencilState(&depthStencilDesc, &m_depth_stencil_state);
		CHECK_HR(hr);
	}

	void DepthStencilState::cleanup()
	{
		SAFE_RELEASE(m_depth_stencil_state);
	}

	void DepthStencilState::set_main()
	{
		ce::state::kcontext->get_context()->OMSetDepthStencilState(m_depth_stencil_state, 1);
	}

}
