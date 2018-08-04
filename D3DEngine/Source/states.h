#pragma once

#include <memory>

/* 
TODO: 
- blend state
- sampler state
*/


class BlendState
{
public:
	BlendState();
	~BlendState();

	void init();
	void cleanup();

	void use();

private:
	struct BlendStateBuffers;
	std::unique_ptr<BlendStateBuffers> m_buffers;

};