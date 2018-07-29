#pragma once
#include <array>
#include <vector>
#include <Windows.h>

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

private:
	void keydown(int key);
	void keyup(int key);

	friend class Window;

private:
	std::array<char, 256> m_keys = {false};
};