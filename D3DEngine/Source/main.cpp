#include <iostream>
#include "window.h"
#include "input.h"
#include "renderdevice.h"
#include "renderer.h"

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
	AdapterInfo GPU = RenderDevice::getAdapterInfo();
	std::cout << "GPU description: "<< GPU.Description << std::endl;
	std::cout << "GPU Vendor ID: " << GPU.VendorId << std::endl;
	std::cout << "GPU Device ID: " << GPU.DeviceId << std::endl;
	std::cout << "GPU SubSys ID: " << GPU.SubSysId << std::endl;
	std::cout << "GPU Revision: " << GPU.Revision << std::endl;
	std::cout << "GPU Mem (ded): " << GPU.DedicatedVideoMemory / 1024 / 1024 << std::endl;
	std::cout << "System Mem (ded): " << GPU.DedicatedSystemMemory / 1024 / 1024 << std::endl;
	std::cout << "Shared Mem: " << GPU.SharedSystemMemory / 1024 / 1024 << std::endl;
	std::cout << "---------------------" << std::endl << std::endl;
}

void loadShaders()
{
	Shader* simpleShader;
	VertexLayout simpleLayout;

	simpleLayout.push<float>(4, "POSITION");
	simpleLayout.push<float>(3, "COLOR");

	simpleShader = new Shader;
	simpleShader->init("Resources/Shaders/simple.vs", "Resources/Shaders/simple.ps", simpleLayout, sizeof(DirectX::XMFLOAT4));
	ShaderFactory::addShader("Simple", simpleShader);
}

int main()
{
	std::cout << "hello world..." << std::endl;
	experimental();
	
	Window wind;
	InputSystem inputsys;
	RenderDevice d3d11Device;
	Renderer renderer;
	Model model;
	DirectX::XMFLOAT4 posOffset;

	wind.init("D3D11Engine");
	wind.setInputSystem(&inputsys);
	d3d11Device.init(RenderDevice::matchOutputMode(800, 600), wind);
	loadShaders();
	renderer.init();
	model.init();
	posOffset = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	while (!wind.isClosed())
	{
		wind.update();

		if (inputsys.isKeyDown(VK_ESCAPE))
			wind.close();

		// toggle fullscreen mode
		if (inputsys.isKeyPressed('F') || inputsys.isKeyPressed(VK_F11))
		{
			static bool fullscreen = false;
			fullscreen = !fullscreen;
			std::cout << (fullscreen ? "Going fullscreen" : "Going windowed") << std::endl;
			wind.setFullscreenState(fullscreen);
			d3d11Device.setFullscreenState(fullscreen);
		}

		const float moveSpeed = 0.1f;
		if (inputsys.isKeyDown(VK_RIGHT))
			posOffset.x += moveSpeed;
		if (inputsys.isKeyDown(VK_LEFT))
			posOffset.x -= moveSpeed;
		if (inputsys.isKeyDown(VK_UP))
			posOffset.y += moveSpeed;
		if (inputsys.isKeyDown(VK_DOWN))
			posOffset.y -= moveSpeed;

		std::cout << posOffset.x << " " << posOffset.y << std::endl;

		d3d11Device.beginScene(0.25f, 0.5f, 0.25f, 1.0f);

		if (inputsys.isKeyDown('G'))
			d3d11Device.beginScene(0.25f, 0.25f, 0.25f, 1.0f);

		
		renderer.render(model, posOffset);

		d3d11Device.endScene();

		inputsys.end();
		Sleep(50);
	}

	model.cleanup();
	renderer.cleanup();
	ShaderFactory::releaseShaders();
	d3d11Device.cleanup();
	wind.cleanup();

	magnificent_exit();
	return 0;
}