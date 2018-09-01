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

	private:
		ID3D11Buffer* m_buffer = nullptr;
		Type m_type;

	public:
		GpuBuffer();
		~GpuBuffer();

		void init(size_t size, Type type);
		void cleanup();
	};
}