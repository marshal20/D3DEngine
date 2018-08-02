#pragma once
#include <memory>

#include "resourcemanager.h"

class Texture
{
public:
	Texture();
	~Texture();

	void init(unsigned int width, unsigned int height, const char* data, int channels = 4);
	void cleanup();

	static Texture* fromRaw(const Resource::RawImageHandle& imageHandle);

private:
	friend class Model;
	void* getView();

private:
	struct TextureBuffers;
	std::unique_ptr<TextureBuffers> m_buffers;
	int m_width, m_height;
	int m_channels;
};