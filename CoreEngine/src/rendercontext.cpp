#include <CoreEngine\rendercontext.h>

#include <d3d11.h>

#include "window\windowimpl.h"
#include "internalstate.h"

namespace ce
{
	RenderContext::RenderContext()
	{
	}

	RenderContext::~RenderContext()
	{
	}

	void RenderContext::init(const Window* wind, const math::Vec2<int>& size)
	{
		HRESULT hr;
		D3D_FEATURE_LEVEL feature_level;
		IDXGIFactory* p_factory;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ID3D11Texture2D* pBackBuffer;

		feature_level = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
			&feature_level, 1, D3D11_SDK_VERSION, &m_device, NULL, &m_context);
		// TODO: check hr.

		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&p_factory);
		// TODO: check hr.

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = size.x;
		swapChainDesc.BufferDesc.Height = size.y;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = wind->m_impl->get_hWnd();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;
		hr = p_factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapchain);
		// TODO: check hr.

		m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_buffer_view);
		// TODO: check hr.

		pBackBuffer->Release();
		pBackBuffer = nullptr;

		// TODO: Separate depth stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		//
		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		//
		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//
		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//
		hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depth_stencil_state);
		m_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
		// END TODO

		// TODO: Separate depth stencil texture.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		//
		depthBufferDesc.Width = size.x;
		depthBufferDesc.Height = size.y;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		//
		hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depth_texture);
		// TODO: check hr.
		//
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		//
		hr = m_device->CreateDepthStencilView(m_depth_texture, &depthStencilViewDesc, &m_depth_view);
		// TODO: check hr.
		// END TODO
		m_context->OMSetRenderTargets(1, &m_buffer_view, m_depth_view);

		p_factory->Release();
	}

	void RenderContext::cleanup()
	{
		m_depth_view->Release();
		m_depth_texture->Release();
		m_depth_stencil_state->Release();
		m_buffer_view->Release();
		m_swapchain->Release();
		m_context->Release();
		m_device->Release();

	}

	void RenderContext::set_main()
	{
		state::m_render_context = this;
		state::d3d::m_device = m_device;
		state::d3d::m_context = m_context;
	}

	void RenderContext::clear(float r, float g, float b, float a)
	{
		float color[4] = { r, g, b, a };

		m_context->ClearRenderTargetView(m_buffer_view, color);
		m_context->ClearDepthStencilView(m_depth_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	}

	void RenderContext::present()
	{
		m_swapchain->Present(0, 0);
	}

	void RenderContext::resize(const math::Vec2<int>& size)
	{
		// TODO: Resize
		ID3D11Texture2D* pBackBuffer;
		HRESULT hr;

		m_depth_texture->Release();
		m_buffer_view->Release();
		m_depth_stencil_state->Release();
		m_depth_view->Release();

		m_swapchain->ResizeBuffers(1, size.x, size.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_buffer_view);
		// TODO: check hr.

		pBackBuffer->Release();
		pBackBuffer = 0;

		// TODO: Separate depth stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		//
		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		//
		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//
		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//
		hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depth_stencil_state);
		m_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
		// END TODO


		// TODO: Separate depth stencil texture.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		//
		depthBufferDesc.Width = size.x;
		depthBufferDesc.Height = size.y;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		//
		hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depth_texture);
		// TODO: check hr.
		//
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		//
		hr = m_device->CreateDepthStencilView(m_depth_texture, &depthStencilViewDesc, &m_depth_view);
		// TODO: check hr.
		// END TODO

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_context->OMSetRenderTargets(1, &m_buffer_view, m_depth_view);

		//setRestrizerOptions({ RestrizerOptions::CullMode::Back,
		//	RestrizerOptions::FillMode::Solid, false, true, false });



		set_viewport(size);
	}

	void RenderContext::set_viewport(const math::Vec2<int>& size)
	{
		D3D11_VIEWPORT viewport;

		viewport.Width = (float)size.x;
		viewport.Height = (float)size.y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_context->RSSetViewports(1, &viewport);
	}

}

