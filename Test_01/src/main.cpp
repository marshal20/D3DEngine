#include <iostream>
#include <CoreEngine/window.h>
#include <thread>

int main()
{
	ce::Window window;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	while (!window.is_closed())
	{
		ce::math::Vec2<int> pos = window.get_position();
		pos.x += 1;
		window.set_position(pos);
		window.set_name(std::string("Hello (") + 
			std::to_string(pos.x) + ", " + 
			std::to_string(pos.y) + ")");
		window.set_name(window.get_name());
		window.update();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	window.cleanup();

	return 0;
}