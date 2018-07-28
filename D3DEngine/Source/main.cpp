#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "window.h"
#include "input.h"

void magnificent_exit()
{
	std::cout << "exiting";
	for (int i = 0; i < 4; i++)
	{
		Sleep(350);
		std::cout << ".";
	}
}

int main()
{
	std::cout << "hello world..." << std::endl;
	
	Window wind;
	InputSystem inputsys;
	wind.init("D3D11Engine");
	wind.setInputSystem(&inputsys);

	while (!wind.isClosed())
	{
		if (inputsys.isKeydown(VK_ESCAPE))
			wind.close();

		wind.update();
		Sleep(50);
	}

	wind.destroy();

	magnificent_exit();
	return 0;
}