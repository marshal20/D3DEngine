#pragma once
#include <string>
#include <memory>

#include "vertexlayout.h"

class Shader
{
public:
	Shader();
	~Shader();

	void init(const std::string& vertex, const std::string& pixel, const VertexLayout& layout);
	void cleanup();

	void use();

private:
	struct ShaderBuffers;
	std::unique_ptr<ShaderBuffers> m_buffers;
};