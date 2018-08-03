#pragma once
#include <memory>

#include <DirectXMath.h>

#include "texture.h"
#include "mesh.h"


class Model
{
public:
	Model();
	~Model();

	void init(const Mesh& mesh, Texture* texture);
	void cleanup();
	void bind() const;

private:
	friend class Renderer;
	int getIndexCount() const;

private:
	struct ModelBuffers;
	std::unique_ptr<ModelBuffers> m_buffers;
	int m_indexCount = 0;
	Texture* m_texture;

};