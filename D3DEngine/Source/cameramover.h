#pragma once
#include "input.h"
#include "camera.h"

#include <DirectXMath.h>

class CameraMover
{
public:
	CameraMover() = default;
	CameraMover(const CameraMover& other) = default;
	~CameraMover() = default;

	void processInput(const InputSystem& inputsys, double deltaTime);
	void apply(Camera& camera);

	void setLocking(bool enable);

private:
	float m_movementSpeed = 2.0f;
	float m_lockingSpeed = 0.1f;
	DirectX::XMFLOAT3 m_cameraPos = { 0.0f, 0.0f, -5.0f };
	DirectX::XMFLOAT3 m_cameraRot = {0.0f, 0.0f, 0.0f};
	bool m_locking = false;
};