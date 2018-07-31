#pragma once
#include <string>
#include <memory>


class Shader
{
public:
	Shader();
	~Shader();

	void init(const std::string& vertex, const std::string& pixel);
	void cleanup();

	void render(unsigned int indexCount);

private:
	struct ShaderBuffers;
	std::unique_ptr<ShaderBuffers> m_buffers;
};