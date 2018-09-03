#pragma once

#include "../math/vec2.h"
#include "../math/vec4.h"
#include "depthstencilstate.h"
#include "depthtexture.h"
#include "shader.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace ce
{
	class Window;

	class GpuBuffer;

	enum class PrimitiveTopology
	{
		TriangleList = 1, TriangleStrip, LineList, LineStrip
	};

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

		static void clear(const math::Vec4<float>& color);
		static void present();

		static void resize(const math::Vec2<int>& size);
		static void set_viewport(const math::Vec2<int>& size);

		static void set_primitive_topology(const PrimitiveTopology primitive_topology);
		static void set_constant_buffer(Shader::Type shader_type, unsigned int slot, const GpuBuffer* constant_buffer);
		static void set_vertex_buffer(unsigned int slot, const GpuBuffer* vertex_buffer, unsigned int stride, unsigned int offset);
		static void set_index_buffer(const GpuBuffer* index_buffer, unsigned int offset);
		static void draw(unsigned int vertex_count, unsigned int start_location = 0);
		static void draw_indexed(unsigned int index_count, unsigned int start_location = 0);

	private:
		static void init_buffers(const math::Vec2<int>& size);

	private:
		friend class DepthStencilState;
		friend class DepthTexture;
		friend class Shader;
		friend class Layout;
		friend class GpuBuffer;
		friend class RasterizerState;
		friend class Texture2D;
		static ID3D11Device* get_device();
		static ID3D11DeviceContext* get_context();
	};

}
