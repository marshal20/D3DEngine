#pragma once
#include "shader.h"
#include "shaderfactory.h"
#include "model.h"
#include "camera.h"
#include "buffer.h"

#include <directxmath.h>


class Renderer
{
public:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	Renderer();
	~Renderer();

	void init();
	void cleanup();

	void render(const Model& model, const Camera& camera);

private:
	void updateConstantBuffers(Buffer& constantBuffer, const Renderer::MatrixBuffer* value);

private:
	Shader* m_shader;
	Buffer m_constantBuffer;
};