#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "window.h"
#include "input.h"
#include "device.h"

void magnificent_exit()
{
	std::cout << "exiting";
	for (int i = 0; i < 4; i++)
	{
		Sleep(350);
		std::cout << ".";
	}
}

void experimental()
{
	AdapterInfo GPU = Device::getAdapterInfo();
	std::cout << GPU.Description << std::endl;
	std::cout << GPU.VendorId << std::endl;
	std::cout << GPU.DeviceId << std::endl;
	std::cout << GPU.SubSysId << std::endl;
	std::cout << GPU.Revision << std::endl;
	std::cout << GPU.DedicatedVideoMemory / 1024 / 1024 << std::endl;
	std::cout << GPU.DedicatedSystemMemory / 1024 / 1024 << std::endl;
	std::cout << GPU.SharedSystemMemory / 1024 / 1024 << std::endl;
}

int main()
{
	std::cout << "hello world..." << std::endl;
	experimental();
	
	Window wind;
	InputSystem inputsys;
	Device d3d11Device;
	wind.init("D3D11Engine");
	wind.setInputSystem(&inputsys);
	d3d11Device.init(Device::getOutputMode(800, 600), wind);

	while (!wind.isClosed())
	{
		if (inputsys.isKeydown(VK_ESCAPE))
			wind.close();

		wind.update();
		Sleep(50);
	}

	wind.cleanup();

	magnificent_exit();
	return 0;
}