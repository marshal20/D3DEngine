#include <CoreEngine/graphics/rastrizerstate.h>

#include <d3d11.h>

#include <CoreEngine/graphics/rendercontext.h>
#include "../utils/callcheck.h"
#include "../utils/safemem.h"

/*
D3D11_RASTERIZER_DESC rasterDesc;
D3D11_CULL_MODE cullmode;
D3D11_FILL_MODE fillmode;
HRESULT hr;

if (m_buffers->pRasterState)
{
m_buffers->pRasterState->Release();
m_buffers->pRasterState = nullptr;
}

cullmode = D3D11_CULL_BACK;
switch (resOpt.cullmode)
{
case RestrizerOptions::CullMode::Back:
cullmode = D3D11_CULL_BACK;
break;
case RestrizerOptions::CullMode::Front:
cullmode = D3D11_CULL_FRONT;
break;
case RestrizerOptions::CullMode::None:
cullmode = D3D11_CULL_NONE;
break;
default:
break;
}

fillmode = D3D11_FILL_SOLID;
switch (resOpt.fillmode)
{
case RestrizerOptions::FillMode::Solid:
fillmode = D3D11_FILL_SOLID;
break;
case RestrizerOptions::FillMode::Wireframe:
fillmode = D3D11_FILL_WIREFRAME;
break;
default:
break;
}

rasterDesc.AntialiasedLineEnable = false;
rasterDesc.CullMode = cullmode;
rasterDesc.DepthBias = 0;
rasterDesc.DepthBiasClamp = 0.0f;
rasterDesc.DepthClipEnable = true;
rasterDesc.FillMode = fillmode;
rasterDesc.FrontCounterClockwise = resOpt.frontCounterClockwise;
rasterDesc.MultisampleEnable = resOpt.multisampleEnable;
rasterDesc.ScissorEnable = resOpt.scissorEnable;
rasterDesc.SlopeScaledDepthBias = 0.0f;

if (m_buffers->pRasterState)
{
m_buffers->pRasterState->Release();
m_buffers->pRasterState = nullptr;
}

hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_buffers->pRasterState);
checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateRasterizerState(...) failed.");

m_pContext->RSSetState(m_buffers->pRasterState);
*/

namespace ce
{
	RasterizerState::RasterizerState()
	{

	}

	RasterizerState::~RasterizerState()
	{

	}

	void RasterizerState::init(Cull cull_mode, Fill fill_mode, Rotation front_face)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rasterizer_desc;
		D3D11_CULL_MODE d3d_cull_mode;
		D3D11_FILL_MODE d3d_fill_mode;
		BOOL d3d_front_counter_clockwise;

		d3d_cull_mode = D3D11_CULL_BACK;
		switch (cull_mode)
		{
		case RasterizerState::Cull::Back:
			d3d_cull_mode = D3D11_CULL_BACK;
			break;
		case RasterizerState::Cull::Front:
			d3d_cull_mode = D3D11_CULL_FRONT;
			break;
		case RasterizerState::Cull::None:
			d3d_cull_mode = D3D11_CULL_NONE;
			break;
		default:
			break;
		}

		d3d_fill_mode = D3D11_FILL_SOLID;
		switch (fill_mode)
		{
		case RasterizerState::Fill::Solid:
			d3d_fill_mode = D3D11_FILL_SOLID;
			break;
		case RasterizerState::Fill::Wireframe:
			d3d_fill_mode = D3D11_FILL_WIREFRAME;
			break;
		default:
			break;
		}

		d3d_front_counter_clockwise = true;
		switch (front_face)
		{
		case Rotation::CCW:
			d3d_front_counter_clockwise = TRUE;
			break;
		case Rotation::CW:
			d3d_front_counter_clockwise = FALSE;
			break;
		default:
			break;
		}

		ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
		rasterizer_desc.AntialiasedLineEnable = false;
		rasterizer_desc.CullMode = d3d_cull_mode;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0.0f;
		rasterizer_desc.DepthClipEnable = true;
		rasterizer_desc.FillMode = d3d_fill_mode;
		rasterizer_desc.FrontCounterClockwise = d3d_front_counter_clockwise;
		rasterizer_desc.MultisampleEnable = false;
		rasterizer_desc.ScissorEnable = false;
		rasterizer_desc.SlopeScaledDepthBias = 0.0f;

		hr = RenderContext::get_device()->CreateRasterizerState(&rasterizer_desc, &m_resterizer_state);
		CHECK_HR(hr);
	}

	void RasterizerState::cleanup()
	{
		SAFE_RELEASE(m_resterizer_state);
	}

	void RasterizerState::use()
	{
		RenderContext::get_context()->RSSetState(m_resterizer_state);
	}
}
