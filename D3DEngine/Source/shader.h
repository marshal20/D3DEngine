#pragma once
#include <string>
#include <memory>

#include "vertexlayout.h"

class Shader
{
public:
	Shader();
	~Shader();

	void init(const std::string& vertex, const std::string& pixel, const VertexLayout& layout, const size_t constantBufferSize);
	void cleanup();

	void setConstantBufferValue(char* value, const size_t size);
	void render(unsigned int indexCount);

private:
	struct ShaderBuffers;
	std::unique_ptr<ShaderBuffers> m_buffers;
};