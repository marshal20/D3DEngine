#pragma once
#include <directxmath.h>

class Camera
{
public:
	Camera();
	~Camera();

	//void init();
	//void cleanup();

	void setPosition(const DirectX::XMFLOAT3& pos);
	void setRotation(const DirectX::XMFLOAT3& rot);
	void setFov(float valueRad);

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	float getFov() const;

	DirectX::XMMATRIX getView() const;

private:
	DirectX::XMFLOAT3 m_pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fov = 45.0f * 3.14f / 180.0f;
};