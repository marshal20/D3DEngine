#include <iostream>
#include <CoreEngine/window.h>
#include <thread>

int main()
{
	ce::Window window;

	window.init("Hello", ce::math::Vec2<int>(800, 600));
	ce::math::Vec2<int> pos = { 50, 50 };
	while (!window.isClosed())
	{
		pos = window.getPosition();
		pos.x += 1;
		window.setPosition(pos);
		window.update();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	window.cleanup();

	return 0;
}