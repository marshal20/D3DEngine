#include "camera.h"


Camera::Camera()
{
	 
}

Camera::~Camera()
{

}

void Camera::setPosition(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
}

void Camera::setRotation(const DirectX::XMFLOAT3& rot)
{
	m_rot = rot;
}

void Camera::setFov(float valueRad)
{
	m_fov = valueRad;
}

DirectX::XMFLOAT3 Camera::getPosition() const
{
	return m_pos;
}

DirectX::XMFLOAT3 Camera::getRotation() const
{
	return m_rot;
}

float Camera::getFov() const
{
	return m_fov;
}

DirectX::XMMATRIX Camera::getView() const
{
	DirectX::XMFLOAT3 up, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;


	up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	upVector = XMLoadFloat3(&up);

	positionVector = XMLoadFloat3(&m_pos);

	lookAt = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rot.x * 0.0174532925f;
	yaw = m_rot.y * 0.0174532925f;
	roll = m_rot.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	return DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}
