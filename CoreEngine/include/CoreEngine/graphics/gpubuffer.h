#pragma once

struct ID3D11Buffer;

namespace ce
{
	class GpuBuffer
	{
	public:
		enum class Type
		{
			Constant = 1, Vertex, Index
		};

		enum class Usage
		{
			Static = 1, Dynamic
		};

	private:
		ID3D11Buffer* m_buffer = nullptr;
		size_t m_size;
		Type m_type;
		Usage m_usage;

	public:
		GpuBuffer();
		~GpuBuffer();

		void init(size_t size, Type type, Usage usage = Usage::Static);
		void cleanup();

		void* map(bool discard = false);
		void unmap();
		void update(const void* data);

	private:
		friend class RenderContext;
	};
}