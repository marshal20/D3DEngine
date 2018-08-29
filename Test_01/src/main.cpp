#include <iostream>
#include <CoreEngine\window.h>
#include <CoreEngine\rendercontext.h>
#include <thread>

int main()
{
	ce::Window window;
	ce::RenderContext render_context;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	render_context.init(&window);
	render_context.resize(window.get_client_size());

	while (!window.is_closed())
	{
		ce::math::Vec2<int> pos = window.get_position();
		ce::math::Vec2<int> size = window.get_size();
		std::string name = std::string("Hello (") +
			std::to_string(pos.x) + ", " +
			std::to_string(pos.y) + ") " +
			std::to_string(size.x) + "x" +
			std::to_string(size.y);
		window.set_name(name);
		window.set_name(window.get_name());

		render_context.clear(1.0f, 0.0f, 0.0f, 1.0f);
		render_context.present();

		window.update();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	window.cleanup();
	render_context.cleanup();

	return 0;
}