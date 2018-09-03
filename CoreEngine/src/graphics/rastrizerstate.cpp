#include <CoreEngine/graphics/rastrizerstate.h>

#include <d3d11.h>

#include <CoreEngine/graphics/rendercontext.h>
#include "../utils/callcheck.h"
#include "../utils/safemem.h"

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