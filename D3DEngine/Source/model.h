#pragma once
#include <memory>

#include <DirectXMath.h>

#include "mesh.h"
#include "buffer.h"
#include "texture.h"
#include "transform.h"


class Model
{
public:
	Model() = default;
	Model(const Mesh& mesh, Texture* texture);
	~Model() = default;

	void bind() const;

	Transform& transform();

private:
	friend class Renderer;
	int getIndexCount() const;
	DirectX::XMMATRIX getTransformMatrix() const;

private:
	std::unique_ptr<Buffer> m_vertexBuffer;
	std::unique_ptr<Buffer> m_indexBuffer;
	int m_indexCount = 0;
	Texture* m_texture;
	Transform m_transform;
};