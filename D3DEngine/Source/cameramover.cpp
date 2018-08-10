#include "cameramover.h"

DirectX::XMFLOAT3 normalize(const DirectX::XMFLOAT3& value);

void CameraMover::processInput(const InputSystem& inputsys, double deltaTime)
{
	// Rotation
	if (m_locking) {
		DirectX::XMFLOAT2 deltaMousePos = inputsys.getDeltaMousePos();
		m_cameraRot.x += deltaMousePos.y * m_lockingSpeed;
		m_cameraRot.y += deltaMousePos.x * m_lockingSpeed;
	}

	// Rotation Matrix
	DirectX::XMMATRIX rotationMatrix;
	float pitch = m_cameraRot.x * 0.0174532925f;
	float yaw = m_cameraRot.y * 0.0174532925f;
	float roll = m_cameraRot.z * 0.0174532925f;
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Position
	DirectX::XMFLOAT3 movementDirection = { 0.0f, 0.0f, 0.0f };
	if (inputsys.isKeyDown(VK_RIGHT))
		movementDirection.x += 1.0f;
	if (inputsys.isKeyDown(VK_LEFT))
		movementDirection.x -= 1.0f;
	if (inputsys.isKeyDown(VK_UP))
		movementDirection.z += 1.0f;
	if (inputsys.isKeyDown(VK_DOWN))
		movementDirection.z -= 1.0f;
	movementDirection = normalize(movementDirection);

	DirectX::XMVECTOR movementVector = DirectX::XMLoadFloat3(&movementDirection);
	movementVector = DirectX::XMVector3TransformCoord(movementVector, rotationMatrix);
	movementDirection.x = DirectX::XMVectorGetX(movementVector);
	movementDirection.y = DirectX::XMVectorGetY(movementVector);
	movementDirection.z = DirectX::XMVectorGetZ(movementVector);
	m_cameraPos.x += movementDirection.x * m_movementSpeed * deltaTime;
	m_cameraPos.y += movementDirection.y * m_movementSpeed * deltaTime;
	m_cameraPos.z += movementDirection.z * m_movementSpeed * deltaTime;
}

void CameraMover::apply(Camera& camera)
{
	camera.setPosition(m_cameraPos);
	camera.setRotation(m_cameraRot);
}

void CameraMover::setLocking(bool enable)
{
	m_locking = enable;
}


// HELPER FUNCTIONS

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