#include <CoreEngine/graphics/shader.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "../utils/safemem.h"
#include "../utils/callcheck.h"
#include "state/internalstate.h"

namespace ce
{
	const char* shader_type_to_name(const Shader::Type type);

	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
	}

	void Shader::load_from_file(const char* filename, const char* entry_point, const Type type)
	{

	}

	void Shader::load_from_memory(const char* text, const char* entry_point, const Type type)
	{
		HRESULT hr;
		ID3D10Blob* shader_blob;
		ID3D10Blob* message_blob;

		m_type = type;

		hr = D3DCompile(text, strlen(text), NULL, NULL, NULL, entry_point, shader_type_to_name(type),
			D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader_blob, &message_blob);
		CHECK_HR(hr);

		SAFE_RELEASE(message_blob);

		switch (type)
		{
		case Type::Vertex:
			hr = state::kcontext->get_device()->CreateVertexShader(shader_blob, shader_blob->GetBufferSize(),
				nullptr, (ID3D11VertexShader**)&m_shader_handle);
			break;

		case Type::Pixel:
			hr = state::kcontext->get_device()->CreatePixelShader(shader_blob, shader_blob->GetBufferSize(),
				nullptr, (ID3D11PixelShader**)&m_shader_handle);
			break;
		}
		CHECK_HR(hr);

		SAFE_RELEASE(shader_blob);
	}

	void Shader::cleanup()
	{
		SAFE_RELEASE(m_shader_handle);
	}

	void Shader::use()
	{
		switch (m_type)
		{
		case Type::Vertex:
			state::kcontext->get_context()->VSSetShader((ID3D11VertexShader*)m_shader_handle, NULL, 0);
			break;

		case Type::Pixel:
			state::kcontext->get_context()->PSSetShader((ID3D11PixelShader*)m_shader_handle, NULL, 0);
			break;
		}
	}

	// HELPER FUNCTIONS

	const char* shader_type_to_name(const Shader::Type type)
	{
		switch (type)
		{
		case Shader::Type::Vertex: return "vs_5_0";
		case Shader::Type::Pixel: return "ps_5_0";
		}
	}
}