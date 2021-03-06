#include <CoreEngine/graphics/rendercontext.h>

#include <d3d11.h>

#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/gpubuffer.h>
#include "../window/windowimpl.h"
#include "../utils/safemem.h"
#include "../utils/callcheck.h"

namespace ce
{

	ID3D11Device* RenderContext::m_device = nullptr;
	ID3D11DeviceContext* RenderContext::m_context = nullptr;
	IDXGISwapChain* RenderContext::m_swapchain = nullptr;
	DepthStencilState RenderContext::m_default_depth_stencil_state;
	DepthTexture RenderContext::m_depth_texture;
	ID3D11RenderTargetView* RenderContext::m_buffer_view = nullptr;
	math::Vec2<int> RenderContext::m_buffer_size = { 0, 0 };

	void RenderContext::init(const Window* wind, const math::Vec2<int>& size)
	{
		HRESULT hr;
		D3D_FEATURE_LEVEL feature_level;
		IDXGIFactory* p_factory;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		m_buffer_size = size;

		feature_level = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
			&feature_level, 1, D3D11_SDK_VERSION, &m_device, NULL, &m_context);
		CHECK_HR(hr);

		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&p_factory);
		CHECK_HR(hr);

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
		CHECK_HR(hr);

		SAFE_RELEASE(p_factory);

		init_buffers(size);
	}

	void RenderContext::cleanup()
	{
		m_default_depth_stencil_state.cleanup();
		m_depth_texture.cleanup();

		SAFE_RELEASE(m_buffer_view);

		SAFE_RELEASE(m_swapchain);
		SAFE_RELEASE(m_context);
		SAFE_RELEASE(m_device);
	}

	void RenderContext::clear(const math::Vec4<float>& color)
	{
		float float_color[4] = { color.r, color.g, color.b, color.a };

		m_context->ClearRenderTargetView(m_buffer_view, float_color);
		m_context->ClearDepthStencilView(m_depth_texture.get_view(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RenderContext::present()
	{
		m_swapchain->Present(0, 0);
	}

	void RenderContext::resize(const math::Vec2<int>& size)
	{
		if (size != m_buffer_size)
		{
			m_buffer_size = size;
			init_buffers(size);
		}
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

	void RenderContext::set_primitive_topology(const PrimitiveTopology primitive_topology)
	{
		D3D11_PRIMITIVE_TOPOLOGY d3d_primitive_topology;

		switch (primitive_topology)
		{
		case PrimitiveTopology::TriangleList:
			d3d_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case PrimitiveTopology::TriangleStrip:
			d3d_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		case PrimitiveTopology::LineList:
			d3d_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PrimitiveTopology::LineStrip:
			d3d_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		default: d3d_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			break;
		}

		m_context->IASetPrimitiveTopology(d3d_primitive_topology);
	}

	void RenderContext::set_constant_buffer(Shader::Type shader_type, unsigned int slot, const GpuBuffer* constant_buffer)
	{
		switch (shader_type)
		{
		case Shader::Type::Vertex:
			m_context->VSSetConstantBuffers(slot, 1, &constant_buffer->m_buffer);
			break;
		case Shader::Type::Pixel:
			m_context->PSSetConstantBuffers(slot, 1, &constant_buffer->m_buffer);
			break;
		default:
			break;
		}
	}

	void RenderContext::set_vertex_buffer(unsigned int slot, const GpuBuffer* vertex_buffer, unsigned int stride, unsigned int offset)
	{
		m_context->IASetVertexBuffers(slot, 1, &vertex_buffer->m_buffer, &stride, &offset);
	}

	void RenderContext::set_index_buffer(const GpuBuffer* index_buffer, unsigned int offset)
	{
		m_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, offset);
	}

	void RenderContext::draw(unsigned int vertex_count, unsigned int start_location)
	{
		m_context->Draw(vertex_count, start_location);
	}

	void RenderContext::draw_indexed(unsigned int index_count, unsigned int start_location)
	{
		m_context->DrawIndexed(index_count, start_location, 0);
	}

	// PRIVATE

	void RenderContext::init_buffers(const math::Vec2<int>& size)
	{
		ID3D11Texture2D* pBackBuffer;
		HRESULT hr;

		m_default_depth_stencil_state.cleanup();
		m_depth_texture.cleanup();
		SAFE_RELEASE(m_buffer_view);

		m_swapchain->ResizeBuffers(1, size.x, size.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_buffer_view);
		CHECK_HR(hr);

		SAFE_RELEASE(pBackBuffer);

		// Depth stencil state
		m_default_depth_stencil_state.init(true, true);
		m_default_depth_stencil_state.use();

		m_depth_texture.create(size);

		m_context->OMSetRenderTargets(1, &m_buffer_view, m_depth_texture.get_view());
	}

	ID3D11Device* RenderContext::get_device()
	{
		return m_device;
	}

	ID3D11DeviceContext* RenderContext::get_context()
	{
		return m_context;
	}

}
