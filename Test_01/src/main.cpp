#include <iostream>
#include <CoreEngine/math/vec2.h>
#include <CoreEngine/math/vec4.h>
#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/rendercontext.h>
#include <CoreEngine/filesystem/image.h>
#include <CoreEngine/graphics/texture2d.h>
#include <CoreEngine/rendersystem/sprite.h>
#include <CoreEngine/rendersystem/renderer2d.h>
#include <thread>

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
		if (sin(time) > 0.0f)
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
