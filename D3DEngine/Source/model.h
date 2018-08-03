#pragma once
#include <memory>

#include <DirectXMath.h>

#include "texture.h"
#include "mesh.h"
#include "transform.h"


class Model
{
public:
	Model();
	~Model();

	void init(const Mesh& mesh, Texture* texture);
	void cleanup();
	void bind() const;

	Transform& transform();

private:
	friend class Renderer;
	int getIndexCount() const;
	DirectX::XMMATRIX getTransformMatrix() const;

private:
	struct ModelBuffers;
	std::unique_ptr<ModelBuffers> m_buffers;
	int m_indexCount = 0;
	Texture* m_texture;
	Transform m_transform;
};