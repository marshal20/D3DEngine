#include <iostream>
#include <CoreEngine/math/vec2.h>
#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/rendercontext.h>
#include <CoreEngine/graphics/rastrizerstate.h>
#include <CoreEngine/graphics/gpubuffer.h>
#include <CoreEngine/graphics/layout.h>
#include <CoreEngine/graphics/shader.h>
#include <thread>
#include <vector>
#include "shadertext.h"

struct Vertex
{
	ce::math::Vec2<float> position;
};

int main()
{
	ce::Window window;
	ce::RasterizerState res_state;
	std::vector<Vertex> vertex_data;
	ce::GpuBuffer vertex_buffer;
	ce::Shader vertex_shader;
	ce::Shader pixel_shader;
	ce::Layout input_layout;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	ce::RenderContext::init(&window);
	ce::RenderContext::resize(window.get_client_size());
	ce::RenderContext::set_viewport(window.get_client_size());

	res_state.init(ce::RasterizerState::Cull::Back, 
		ce::RasterizerState::Fill::Solid, ce::RasterizerState::Rotation::CCW);

	vertex_data.push_back({ {-0.5f, +0.5f} });
	vertex_data.push_back({ {-0.5f, -0.5f} });
	vertex_data.push_back({ {+0.5f, +0.5f} });
	vertex_data.push_back({ {-0.5f, -0.5f} });
	vertex_data.push_back({ {+0.5f, -0.5f} });
	vertex_data.push_back({ {+0.5f, +0.5f} });

	vertex_buffer.init(vertex_data.size() * sizeof(Vertex), 
		ce::GpuBuffer::Type::Vertex, ce::GpuBuffer::Usage::Dynamic);
	vertex_buffer.update(&vertex_data[0]);

	input_layout.push({ "POSITION", ce::Layout::Element::Type::FLOAT, 2 });
	input_layout.create();

	vertex_shader.load_from_memory(SIMPLE_SHADER_VS, "vs_main", ce::Shader::Type::Vertex);
	pixel_shader.load_from_memory(SIMPLE_SHADER_PS, "ps_main", ce::Shader::Type::Pixel);

	while (!window.is_closed())
	{
		window.update();

		ce::RenderContext::clear(0.75f, 0.75f, 0.75f, 1.0f);
		// DRAW
		res_state.use();
		vertex_shader.use();
		pixel_shader.use();
		input_layout.use();
		ce::RenderContext::set_vertex_buffer(0, &vertex_buffer, sizeof(Vertex), 0);
		ce::RenderContext::set_primitive_topology(ce::PrimitiveTopology::TriangleList);
		ce::RenderContext::draw(6);
		ce::RenderContext::present();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	vertex_buffer.cleanup();
	input_layout.cleanup();
	vertex_shader.cleanup();
	pixel_shader.cleanup();
	res_state.cleanup();
	window.cleanup();
	ce::RenderContext::cleanup();

	return 0;
}