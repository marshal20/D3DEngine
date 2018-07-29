#include "input.h"
#include <iostream>

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::end()
{
	// TODO:
	m_keys = { false };
}

bool InputSystem::isKeydown(int key)
{
	return m_keys[key];
}

bool InputSystem::isKeyup(int key)
{
	return !m_keys[key];
}

void InputSystem::keydown(int key)
{
	m_keys[key] = true;
}

void InputSystem::keyup(int key)
{
	m_keys[key] = false;
}
