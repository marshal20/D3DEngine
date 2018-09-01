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
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_context = nullptr;
		IDXGISwapChain* m_swapchain = nullptr;

		DepthStencilState m_default_depth_stencil_state;
		DepthTexture m_depth_texture;
		ID3D11RenderTargetView* m_buffer_view = nullptr;

		math::Vec2<int> m_buffer_size = { 0, 0 };

	public:
		RenderContext();
		RenderContext(const RenderContext& other) = delete;
		~RenderContext();

		void init(const Window* wind, const math::Vec2<int>& size = {800, 600});
		void cleanup();

		void set_main();
		void clear(float r, float g, float b, float a);
		void present();

		void resize(const math::Vec2<int>& size);
		void set_viewport(const math::Vec2<int>& size);

	private:
		void init_buffers(const math::Vec2<int>& size);

	private:
		friend class DepthStencilState;
		friend class DepthTexture;
		friend class Shader;
		friend class Layout;
		friend class GpuBuffer;
		ID3D11Device* get_device() const;
		ID3D11DeviceContext* get_context() const;
	};
}

