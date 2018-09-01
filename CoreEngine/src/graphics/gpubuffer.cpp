#include <CoreEngine/graphics/gpubuffer.h>

#include <d3d11.h>

#include "state/internalstate.h"
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
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = get_bind_flag(type);
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = NULL;
		buffer_desc.StructureByteStride = NULL;
		hr = state::kcontext->get_device()->CreateBuffer(&buffer_desc, NULL, &m_buffer);
		CHECK_HR(hr);
	}

	void GpuBuffer::cleanup()
	{
		SAFE_RELEASE(m_buffer);
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