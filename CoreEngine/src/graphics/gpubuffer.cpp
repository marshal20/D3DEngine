#include <CoreEngine/graphics/gpubuffer.h>

#include <d3d11.h>

#include <CoreEngine/graphics/rendercontext.h>
#include "../utils/callcheck.h"
#include "../utils/safemem.h"

namespace ce
{
	UINT get_bind_flag(const GpuBuffer::Type type);

	GpuBuffer::GpuBuffer()
	{
	}

	GpuBuffer::~GpuBuffer()
	{
	}

	void GpuBuffer::init(size_t size, Type type)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC buffer_desc;

		m_type = type;

		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth = max(16, size);
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = get_bind_flag(type);
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = NULL;
		buffer_desc.StructureByteStride = NULL;
		hr = RenderContext::get_device()->CreateBuffer(&buffer_desc, NULL, &m_buffer);
		CHECK_HR(hr);
	}

	void GpuBuffer::cleanup()
	{
		SAFE_RELEASE(m_buffer);
	}

	void GpuBuffer::update(const void* data, const size_t size)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subsource;

		RenderContext::get_context()->Map(m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_subsource);
		memcpy(mapped_subsource.pData, data, size);
		RenderContext::get_context()->Unmap(m_buffer, NULL);
	}

	// HELPER FUNCTIONS

	UINT get_bind_flag(const GpuBuffer::Type type)
	{
		switch (type)
		{
		case GpuBuffer::Type::Constant: return D3D11_BIND_CONSTANT_BUFFER;
		case GpuBuffer::Type::Vertex: return D3D11_BIND_VERTEX_BUFFER;
		case GpuBuffer::Type::Index: return D3D11_BIND_INDEX_BUFFER;
		default: return 0;
		}
	}

}