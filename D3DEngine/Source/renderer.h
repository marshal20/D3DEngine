#pragma once
#include "shader.h"
#include "shaderfactory.h"
#include "model.h"

#include <directxmath.h>


class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();
	void cleanup();

	void render(const Model& model);

private:
	Shader* m_shader;
};