#include <iostream>

#include "logger.h"
#include "window.h"
#include "input.h"
#include "renderdevice.h"
#include "renderer.h"
#include "camera.h"
#include "cameramover.h"
#include "mesh.h"
#include "transform.h"
#include "framelimiter.h"

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

float clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

class GameSystem
{
public:
	GameSystem() 
	{

	}

	~GameSystem()
	{

	}

	void run();

private:
	void loadShaders();
	void init();
	void cleanup();
	void input();
	void render();

private:
	std::unique_ptr<Window> wind;
	InputSystem inputsys;
	std::unique_ptr<RenderDevice> d3d11Device;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Model> model;
	Texture* modelTex;
	Logger logger;

	FrameLimiter frameLimiter;
	Camera camera;
	CameraMover cameraMover;
	float fovDeg = 45.0f;
	float deltaTime = 0.0f;
};

void GameSystem::loadShaders()
{
	Shader* simpleShader;
	VertexLayout simpleLayout;
	Resource::RawTextHandle vertexShaderContents;
	Resource::RawTextHandle pixelShaderContents;

	vertexShaderContents = Resource::loadTextFile("Resources/Shaders/simple.vs");
	pixelShaderContents = Resource::loadTextFile("Resources/Shaders/simple.ps");

	simpleLayout.push<float>(3, "POSITION");
	simpleLayout.push<float>(3, "NORMAL");
	simpleLayout.push<float>(2, "COORD");

	simpleShader = new Shader;
	simpleShader->init(*vertexShaderContents.text, *pixelShaderContents.text, simpleLayout);
	ShaderFactory::addShader("Simple", simpleShader);
}

void GameSystem::init()
{
	logger.addOut(&std::cout);
	wind = std::make_unique<Window>("D3D11Engine");
	wind->setInputSystem(&inputsys);
	d3d11Device = std::make_unique<RenderDevice>(RenderDevice::matchOutputMode(800, 600), *wind);
	loadShaders();
	renderer = std::make_unique<Renderer>();
	modelTex = Texture::fromRaw(Resource::loadImage("Resources/Images/maze.png"));
	model = std::make_unique<Model>(Resource::loadMesh("Resources/Meshes/cube.obj"), modelTex);
	//model.init(Resource::loadMesh("Resources/Meshes/monkey.obj"), modelTex);
}

void GameSystem::cleanup()
{
	delete modelTex;
	ShaderFactory::releaseShaders();
}

void GameSystem::input()
{
	if (inputsys.isKeyDown(VK_ESCAPE))
		wind->close();

	// toggle fullscreen mode
	if (inputsys.isKeyPressed('F') || inputsys.isKeyPressed(VK_F11))
	{
		static bool fullscreen = false;
		fullscreen = !fullscreen;
		std::cout << (fullscreen ? "Going fullscreen" : "Going windowed") << std::endl;
		wind->setFullscreenState(fullscreen);
		d3d11Device->setFullscreenState(fullscreen);
	}

	// Camera control
	{
		static bool lockingControl = false;
		if (inputsys.isKeyPressed('L')) {
			lockingControl = !lockingControl;
			wind->setLockMouse(lockingControl);
			cameraMover.setLocking(lockingControl);
		}

		cameraMover.processInput(inputsys, deltaTime);
		cameraMover.apply(camera);
	}

	// FOV control
	{
		const float fovSpeed = 4.0f;
		if (inputsys.isKeyDown(VK_ADD))
			fovDeg += fovSpeed * deltaTime;
		if (inputsys.isKeyDown(VK_SUBTRACT))
			fovDeg -= fovSpeed * deltaTime;
		fovDeg = clamp(fovDeg, 45.0f, 90.0f);

		camera.setFov(fovDeg * 3.14f / 180.0f);
	}

	// model transform
	{
		Transform& modelTransform = model->transform();

		static float t = 0.0f;

		float scaleValue = sin(t)*0.5f + 0.5f;
		modelTransform.scale = DirectX::XMFLOAT3(scaleValue, scaleValue, scaleValue);
		modelTransform.rotation = DirectX::XMFLOAT3(t * 15, t * 15, t * 15);
		/*{
			const float movementSpeed = 0.1f;
			DirectX::XMFLOAT3 movementDirection = { 0.0f, 0.0f, 0.0f };
			if (inputsys.isKeyDown('D'))
				movementDirection.x += 1.0f;
			if (inputsys.isKeyDown('A'))
				movementDirection.x -= 1.0f;
			if (inputsys.isKeyDown('W'))
				movementDirection.y += 1.0f;
			if (inputsys.isKeyDown('S'))
				movementDirection.y -= 1.0f;;
			movementDirection = normalize(movementDirection);
			modelTransform.position.x += movementDirection.x * movementSpeed;
			modelTransform.position.y += movementDirection.y * movementSpeed;
			modelTransform.position.z += movementDirection.z * movementSpeed;
		}*/

		t += 2.5f * deltaTime;
	}

	// FPS
	{
		static float elapsedTime = 1.0f;
		float lastFrameTime;

		lastFrameTime = (float)frameLimiter.getLastFramTime();
		elapsedTime += lastFrameTime;
		if (elapsedTime > 1.0f)
		{
			std::cout << lastFrameTime * 1000.0f << " ms, @ " << (int)(1.0f / lastFrameTime) << " FPS" << std::endl;
			elapsedTime = 0.0f;
		}
	}

}

void GameSystem::render()
{
	d3d11Device->beginScene(0.25f, 0.25f, 0.25f, 1.0f);

	renderer->render(model.get(), &camera);

	d3d11Device->endScene();
}

void GameSystem::run()
{
	init();

	while (!wind->isClosed())
	{
		deltaTime = (float)frameLimiter.getLastFramTime();
		wind->update();

		input();
		render();

		inputsys.end();
		frameLimiter.end();
	}

	cleanup();
}

int main()
{
	experimental();
	
	GameSystem gameSystem;

	gameSystem.run();

	magnificent_exit();
	return 0;
}