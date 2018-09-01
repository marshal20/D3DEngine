#pragma once

#include "../math/vec2.h"
#include "depthstencilstate.h"
#include "depthtexture.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace ce
{
	class Window;

	class RenderContext
	{
	private:
		static ID3D11Device* m_device;
		static ID3D11DeviceContext* m_context;
		static IDXGISwapChain* m_swapchain;
		static DepthStencilState m_default_depth_stencil_state;
		static DepthTexture m_depth_texture;
		static ID3D11RenderTargetView* m_buffer_view;
		static math::Vec2<int> m_buffer_size;

	public:
		RenderContext() = delete;
		RenderContext(const RenderContext& other) = delete;
		~RenderContext() = delete;

		static void init(const Window* wind, const math::Vec2<int>& size = { 800, 600 });
		static void cleanup();

		static void clear(float r, float g, float b, float a);
		static void present();

		static void resize(const math::Vec2<int>& size);
		static void set_viewport(const math::Vec2<int>& size);

	private:
		static void init_buffers(const math::Vec2<int>& size);

	private:
		friend class DepthStencilState;
		friend class DepthTexture;
		friend class Shader;
		friend class Layout;
		friend class GpuBuffer;
		static ID3D11Device* get_device();
		static ID3D11DeviceContext* get_context();
	};

}

