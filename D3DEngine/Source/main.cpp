#include <iostream>
#include "window.h"
#include "input.h"
#include "renderdevice.h"
#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"

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


DirectX::XMFLOAT3 normalize(const DirectX::XMFLOAT3& value)
{
	DirectX::XMFLOAT3 normalized;

	normalized = value;
	float valueMagnitude = sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
	if (valueMagnitude != 0.0f)
	{
		normalized.x = value.x / valueMagnitude;
		normalized.y = value.y / valueMagnitude;
		normalized.z = value.z / valueMagnitude;
	}

	return normalized;
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

Mesh cubeMesh()
{
	return {
		{
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // bottom left
			{ DirectX::XMFLOAT3(-1.0f, +1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // top left
			{ DirectX::XMFLOAT3(+1.0f, +1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // top right
			{ DirectX::XMFLOAT3(+1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // bottom right

			{ DirectX::XMFLOAT3(-1.0f, -1.0f, 2.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f, +1.0f, 2.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(+1.0f, +1.0f, 2.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(+1.0f, -1.0f, 2.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
		},

		{
			0, 1, 2, 3, 0, 2, // front face
			4, 6, 5, 7, 6, 4, // back face

			3, 2, 6, 7, 3, 6, // right face
			0, 5, 1, 4, 5, 0, // left face

			5, 6, 1, 2, 1, 6, // top face
			0, 7, 4, 7, 0, 3 // bottom face
		}
	};
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
	Window wind;
	InputSystem inputsys;
	RenderDevice d3d11Device;
	Renderer renderer;
	Model model;
	Texture* modelTex;
	Camera camera;
	DirectX::XMFLOAT3 cameraPos;
	float fovDeg = 45.0f;
};

void GameSystem::loadShaders()
{
	Shader* simpleShader;
	VertexLayout simpleLayout;

	simpleLayout.push<float>(3, "POSITION");
	simpleLayout.push<float>(2, "COORD");

	simpleShader = new Shader;
	simpleShader->init("Resources/Shaders/simple.vs", "Resources/Shaders/simple.ps", simpleLayout);
	ShaderFactory::addShader("Simple", simpleShader);
}

void GameSystem::init()
{
	wind.init("D3D11Engine");
	wind.setInputSystem(&inputsys);
	d3d11Device.init(RenderDevice::matchOutputMode(800, 600), wind);
	loadShaders();
	renderer.init();
	modelTex = Texture::fromRaw(Resource::loadImage("Resources/Images/test.png"));
	model.init(cubeMesh(), modelTex);
	cameraPos = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
}

void GameSystem::cleanup()
{
	modelTex->cleanup();
	delete modelTex;
	model.cleanup();
	renderer.cleanup();
	ShaderFactory::releaseShaders();
	d3d11Device.cleanup();
	wind.cleanup();
}

void GameSystem::input()
{
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

	// camera movement
	{
		const float movementSpeed = 0.1f;
		DirectX::XMFLOAT3 movementDirection = { 0.0f, 0.0f, 0.0f };
		if (inputsys.isKeyDown(VK_RIGHT))
			movementDirection.x += 1.0f;
		if (inputsys.isKeyDown(VK_LEFT))
			movementDirection.x -= 1.0f;
		if (inputsys.isKeyDown(VK_UP))
			movementDirection.y += 1.0f;
		if (inputsys.isKeyDown(VK_DOWN))
			movementDirection.y -= 1.0f;
		if (inputsys.isKeyDown('I'))
			movementDirection.z += 1.0f;
		if (inputsys.isKeyDown('O'))
			movementDirection.z -= 1.0f;
		movementDirection = normalize(movementDirection);
		cameraPos.x += movementDirection.x * movementSpeed;
		cameraPos.y += movementDirection.y * movementSpeed;
		cameraPos.z += movementDirection.z * movementSpeed;
	}

	// fov control
	{
		if (inputsys.isKeyDown(VK_ADD))
			fovDeg += 1.0f;
		if (inputsys.isKeyDown(VK_SUBTRACT))
			fovDeg -= 1.0f;
		fovDeg = clamp(fovDeg, 45.0f, 90.0f);

		camera.setPosition(cameraPos);
		camera.setFov(fovDeg * 3.14f / 180.0f);
	}

	// model tranform
	{
		Transform& modelTransform = model.transform();

		static float t = 0.0f;

		modelTransform.scale = DirectX::XMFLOAT3(sin(t)*0.5f + 0.5f, sin(t)*0.5f + 0.5f, sin(t)*0.5f + 0.5f);
		modelTransform.rotation = DirectX::XMFLOAT3(t * 15, t * 15, t * 15);
		{
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
		}

		t += 0.1f;
	}
}

void GameSystem::render()
{
	d3d11Device.beginScene(0.25f, 0.25f, 0.25f, 1.0f);

	renderer.render(model, camera);

	d3d11Device.endScene();
}

void GameSystem::run()
{
	init();

	while (!wind.isClosed())
	{
		wind.update();

		input();
		render();

		inputsys.end();
		Sleep(50);
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