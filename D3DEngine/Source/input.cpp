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
}

bool InputSystem::isKeyDown(int key)
{
	return m_keys[key] == keyState::down || m_keys[key] == keyState::pressed;
}

bool InputSystem::isKeyUp(int key)
{
	return m_keys[key] == keyState::up || m_keys[key] == keyState::released;
}

bool InputSystem::isKeyPressed(int key)
{
	return m_keys[key] == keyState::pressed;
}

bool InputSystem::isKeyReleased(int key)
{
	return m_keys[key] == keyState::released;
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
