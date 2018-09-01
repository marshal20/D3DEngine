#include <iostream>
#include <CoreEngine/math/vec2.h>
#include <CoreEngine/window/window.h>
#include <CoreEngine/graphics/rendercontext.h>
#include <thread>

int main()
{
	ce::Window window;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	ce::RenderContext::init(&window);
	ce::RenderContext::resize(window.get_client_size());
	ce::RenderContext::set_viewport(window.get_client_size());

	while (!window.is_closed())
	{
		window.update();

		ce::RenderContext::clear(0.75f, 0.75f, 0.75f, 1.0f);
		ce::RenderContext::present();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	window.cleanup();
	ce::RenderContext::cleanup();

	return 0;
}