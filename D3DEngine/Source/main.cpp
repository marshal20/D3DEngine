#include <iostream>
#include <Windows.h>
#include "window.h"
#include "input.h"

int main()
{
	std::cout << "hello world..." << std::endl;
	
	Window wind;
	InputSystem inputsys;
	wind.init();
	wind.setInputSystem(&inputsys);

	while (!wind.isClosed())
	{
		if (inputsys.isKeydown(VK_ESCAPE))
			wind.close();

		wind.update();
		Sleep(50);
	}

	wind.destroy();

	std::cout << "exiting" << std::endl;
	Sleep(1000);
	return 0;
}