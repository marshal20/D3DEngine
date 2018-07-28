#include <iostream>
#include "window.h"
#include <Windows.h>

int main()
{
	std::cout << "hello world..." << std::endl;
	
	Window wind;
	wind.init();

	while (!wind.isClosed())
	{
		wind.update();
		Sleep(50);
	}

	wind.destroy();

	std::cout << "exiting" << std::endl;
	std::cin.get();
	return 0;
}