#include <iostream>
#include <CoreEngine/math/vec2.h>
#include <CoreEngine/math/vec4.h>
#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/rendercontext.h>
#include <CoreEngine/graphics/rastrizerstate.h>
#include <CoreEngine/filesystem/image.h>
#include <CoreEngine/graphics/texture2d.h>
#include <CoreEngine/graphics/gpubuffer.h>
#include <CoreEngine/graphics/layout.h>
#include <CoreEngine/graphics/shader.h>
#include <CoreEngine/rendersystem/sprite.h>
#include <CoreEngine/rendersystem/renderer2d.h>
#include <thread>
#include <vector>
#include "shadertext.h"

int main()
{
	ce::Window window;
	ce::Image image;
	ce::Texture2D texture;
	ce::Renderer2D renderer2d;
	ce::Sprite my_sprite;
	float time = 0.0f;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	ce::RenderContext::init(&window);
	ce::RenderContext::resize(window.get_client_size());
	ce::RenderContext::set_viewport(window.get_client_size());

	renderer2d.init();

	image.load_from_file("res/Images/maze.png");
	texture.create(image.get_size());
	texture.update_from_image(&image);
	texture.set_sample_options({ ce::Texture2D::Sample::Filter::Bilinear,
		ce::Texture2D::Sample::Wrap::Repeat, ce::Texture2D::Sample::Wrap::Repeat });
	image.cleanup();

	while (!window.is_closed())
	{
		window.update();
		if (!window.is_closed())
		{
			ce::RenderContext::resize(window.get_client_size());
			ce::RenderContext::set_viewport(window.get_client_size());
		}

		my_sprite.position = { -0.5f + 0.5f*cos(-time), -0.5f + 0.5f*sin(-time) };
		my_sprite.size = { 1.0f, 1.0f };
		my_sprite.color = { sin(time*0.9f) * 0.5f + 0.5f, cos(time*0.5f) * 0.5f + 0.5f,
			sin(time*0.7f) * cos(time*2.0f) * 0.5f + 0.5f, 1.0f };
		if (time > 2.0f)
		{
			my_sprite.texture = nullptr;
		}
		else
		{
			my_sprite.texture = &texture;
		}

		renderer2d.clear();
		renderer2d.draw(&my_sprite);
		renderer2d.update();

		ce::RenderContext::clear({ 0.75f, 0.75f, 0.75f, 1.0f });
		renderer2d.render();
		ce::RenderContext::present();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		time += 0.05f;
	}

	renderer2d.cleanup();
	texture.cleanup();
	window.cleanup();
	ce::RenderContext::cleanup();

	return 0;
}

/*
struct Vertex
{
	ce::math::Vec2<float> position;
	ce::math::Vec2<float> tex_coords;
};

int main()
{
	ce::Window window;
	ce::RasterizerState res_state;
	std::vector<Vertex> vertex_data;
	ce::GpuBuffer vertex_buffer;
	ce::math::Vec4<float> color;
	ce::GpuBuffer ps_constant_buffer;
	ce::Shader vertex_shader;
	ce::Shader pixel_shader;
	ce::Layout input_layout;
	ce::Image image;
	ce::Texture2D texture;
	float time = 0.0f;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	ce::RenderContext::init(&window);
	ce::RenderContext::resize(window.get_client_size());
	ce::RenderContext::set_viewport(window.get_client_size());

	res_state.init(ce::RasterizerState::Cull::Back, 
		ce::RasterizerState::Fill::Solid, ce::RasterizerState::Rotation::CCW);

	vertex_data.push_back({ {-0.5f, +0.5f}, {+0.0f, +0.0f} });
	vertex_data.push_back({ {-0.5f, -0.5f}, {+0.0f, +1.0f} });
	vertex_data.push_back({ {+0.5f, +0.5f}, {+1.0f, +0.0f} });
	vertex_data.push_back({ {-0.5f, -0.5f}, {+0.0f, +1.0f} });
	vertex_data.push_back({ {+0.5f, -0.5f}, {+1.0f, +1.0f} });
	vertex_data.push_back({ {+0.5f, +0.5f}, {+1.0f, +0.0f} });

	vertex_buffer.init(vertex_data.size() * sizeof(Vertex), 
		ce::GpuBuffer::Type::Vertex, ce::GpuBuffer::Usage::Dynamic);
	vertex_buffer.update(&vertex_data[0]);

	ps_constant_buffer.init(4 * sizeof(float), ce::GpuBuffer::Type::Constant, ce::GpuBuffer::Usage::Static);

	input_layout.push({ "POSITION", ce::Layout::Element::Type::FLOAT, 2 });
	input_layout.push({ "TEXCOORDS", ce::Layout::Element::Type::FLOAT, 2 });
	input_layout.create();

	vertex_shader.load_from_memory(SIMPLE_SHADER_VS, "vs_main", ce::Shader::Type::Vertex);
	pixel_shader.load_from_memory(SIMPLE_SHADER_PS, "ps_main", ce::Shader::Type::Pixel);

	image.load_from_file("res/Images/maze.png");
	texture.create(image.get_size());
	texture.update_from_image(&image);
	texture.set_sample_options({ ce::Texture2D::Sample::Filter::Bilinear, 
		ce::Texture2D::Sample::Wrap::Repeat, ce::Texture2D::Sample::Wrap::Repeat });
	image.cleanup();

	while (!window.is_closed())
	{
		window.update();
		if (!window.is_closed())
		{
			ce::RenderContext::resize(window.get_client_size());
			ce::RenderContext::set_viewport(window.get_client_size());
		}

		ce::RenderContext::clear({ 0.75f, 0.75f, 0.75f, 1.0f });
		// DRAW
		res_state.use();
		vertex_shader.use();
		pixel_shader.use();
		input_layout.use();
		ce::RenderContext::set_vertex_buffer(0, &vertex_buffer, sizeof(Vertex), 0);
		texture.use(0);
		color = { sin(time*0.9f) * 0.5f + 0.5f, cos(time*0.5f) * 0.5f + 0.5f,
			sin(time*0.7f) * cos(time*2.0f) * 0.5f + 0.5f, 1.0f };
		ps_constant_buffer.update(&color);
		ce::RenderContext::set_constant_buffer(ce::Shader::Type::Pixel, 0, &ps_constant_buffer);
		ce::RenderContext::set_primitive_topology(ce::PrimitiveTopology::TriangleList);
		ce::RenderContext::draw(6);
		ce::RenderContext::present();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		time += 0.05f;
	}

	texture.cleanup();
	vertex_buffer.cleanup();
	input_layout.cleanup();
	vertex_shader.cleanup();
	pixel_shader.cleanup();
	res_state.cleanup();
	window.cleanup();
	ce::RenderContext::cleanup();

	return 0;
}
*/