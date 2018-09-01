#include <iostream>
#include <CoreEngine/math/vec.h>
#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/rendercontext.h>
#include <thread>

int main()
{
	ce::Window window;
	ce::RenderContext render_context;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	render_context.init(&window);
	render_context.resize(window.get_client_size());
	render_context.set_viewport(window.get_client_size());

	while (!window.is_closed())
	{
		window.update();

		render_context.clear(0.75f, 0.75f, 0.75f, 1.0f);
		render_context.present();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	window.cleanup();
	render_context.cleanup();

	return 0;
}