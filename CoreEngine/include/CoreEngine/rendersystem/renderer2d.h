#pragma once

#include <vector>
#include <array>

namespace ce
{
	class RasterizerState;
	class Shader;
	class Layout;
	class GpuBuffer;
	struct Sprite;

	class Renderer2D
	{
	private:
		struct Vertex;
		struct Batch;

	private:
		static const size_t MAX_SPRITES = 10000;
		RasterizerState* m_rasterizer_state = nullptr;
		Shader* m_vertex_shader = nullptr;
		Shader* m_pixel_shader = nullptr;
		Layout* m_layout = nullptr;
		GpuBuffer* m_vertex_buffer = nullptr;
		GpuBuffer* m_constant_buffer = nullptr;
		std::vector<Batch> m_batches;
		std::vector<Vertex> m_vertex_array;
		size_t m_cur_index = 0;

	public:
		Renderer2D();
		~Renderer2D();

		void init();
		void cleanup();

		void draw(const Sprite* sprite);

		void update();
		void render();
		void clear();
	};
}