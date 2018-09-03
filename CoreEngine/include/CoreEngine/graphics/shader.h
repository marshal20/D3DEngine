#pragma once

struct ID3D11DeviceChild;

namespace ce
{

	class Shader
	{
	public:
		enum class Type
		{
			Vertex = 1, Pixel
		};

	private:
		Type m_type;
		ID3D11DeviceChild* m_shader_handle = nullptr;

	public:
		Shader();
		~Shader();

		void load_from_file(const char* filename, const char* entry_point, const Type type);
		void load_from_memory(const char* text, const char* entry_point, const Type type);
		void cleanup();

		void use();
	};
}