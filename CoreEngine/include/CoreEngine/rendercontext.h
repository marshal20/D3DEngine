#pragma once

#include "window.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilState; // TODO: separate.
struct ID3D11Texture2D; // TODO: separate.
struct ID3D11DepthStencilView; // TODO: separate.

namespace ce
{
	class RenderContext
	{
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
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_context = nullptr;
		IDXGISwapChain* m_swapchain = nullptr;
		ID3D11RenderTargetView* m_buffer_view = nullptr;
		ID3D11DepthStencilState* m_depth_stencil_state = nullptr; // TODO: separate.
		ID3D11Texture2D* m_depth_texture = nullptr; // TODO: separate.
		ID3D11DepthStencilView* m_depth_view = nullptr; // TODO: separate.
		math::Vec2<int> m_buffer_size = { 0, 0 };
	};
}

