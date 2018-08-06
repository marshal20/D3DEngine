#pragma once
#include <array>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DirectXMath.h>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void end();

	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyPressed(int key);
	bool isKeyReleased(int key);

	DirectX::XMFLOAT2 getMousePos();
	DirectX::XMFLOAT2 getDeltaMousePos();

private:
	friend class Window;
	void keydown(int key);
	void keyup(int key);
	void mousemove(int x, int y, int dx, int dy);

private:
	std::array<char, 256> m_keys = {false};
	int m_x = 0;
	int m_y = 0;
	int m_dx = 0;
	int m_dy = 0;
};