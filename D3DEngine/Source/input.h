#pragma once
#include <array>
#include <Windows.h>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	bool isKeydown(int key);
	bool isKeyup(int key);

private:
	void keydown(int key);
	void keyup(int key);

	friend class Window;

private:
	std::array<bool, 256> m_keys = {false};
};