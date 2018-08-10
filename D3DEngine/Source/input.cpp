#include "input.h"

#include <iostream>

enum keyState
{
	up = 1,
	down,
	pressed,
	released
};

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::end()
{
	for (auto& key : m_keys)
	{
		if (key == keyState::pressed)
			key = keyState::down;

		if (key == keyState::released)
			key = keyState::up;
	}

	m_dx = 0;
	m_dy = 0;
}

bool InputSystem::isKeyDown(int key) const
{
	return m_keys[key] == keyState::down || m_keys[key] == keyState::pressed;
}

bool InputSystem::isKeyUp(int key) const
{
	return m_keys[key] == keyState::up || m_keys[key] == keyState::released;
}

bool InputSystem::isKeyPressed(int key) const
{
	return m_keys[key] == keyState::pressed;
}

bool InputSystem::isKeyReleased(int key) const
{
	return m_keys[key] == keyState::released;
}

DirectX::XMFLOAT2 InputSystem::getMousePos() const
{
	return { (float)m_x, (float)m_y };
}

DirectX::XMFLOAT2 InputSystem::getDeltaMousePos() const
{
	return { (float)m_dx, (float)m_dy };
}

void InputSystem::keydown(int key)
{
	if(m_keys[key] != keyState::down)
		m_keys[key] = keyState::pressed;
}

void InputSystem::keyup(int key)
{
	m_keys[key] = keyState::released;
}

void InputSystem::mousemove(int x, int y, int dx, int dy)
{
	m_dx = dx;
	m_dy = dy;
	m_x = x;
	m_y = y;
}
