#include <CoreEngine/graphics/gpubuffer.h>

#include <d3d11.h>
#include <math.h>

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

	void GpuBuffer::init(size_t size, Type type, Usage usage)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC buffer_desc;
		D3D11_USAGE d3d_usage;
		UINT d3d_cpu_access_flag;

		if (type == Type::Constant)
		{
			size = (int)ceil(float(size) / 16.0f) * 16;
			if (size > D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * 16)
			{
				// TODO: Handle error
			}
		}
		m_size = size;
		m_type = type;
		m_usage = usage;

		switch (usage)
		{
		case Usage::Static:
			d3d_usage = D3D11_USAGE_DEFAULT;
			d3d_cpu_access_flag = 0;
			break;
		case Usage::Dynamic:
			d3d_usage = D3D11_USAGE_DYNAMIC;
			d3d_cpu_access_flag = D3D11_CPU_ACCESS_WRITE;
			break;
		default:
			break;
		}
		
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth = size;
		buffer_desc.Usage = d3d_usage;
		buffer_desc.BindFlags = get_bind_flag(type);
		buffer_desc.CPUAccessFlags = d3d_cpu_access_flag;
		buffer_desc.MiscFlags = NULL;
		buffer_desc.StructureByteStride = NULL;
		hr = RenderContext::get_device()->CreateBuffer(&buffer_desc, NULL, &m_buffer);
		CHECK_HR(hr);
	}

	void GpuBuffer::cleanup()
	{
		SAFE_RELEASE(m_buffer);
	}

	void* GpuBuffer::map(bool discard)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mapped_subsource;
		D3D11_MAP d3d_map;

		if (m_usage == Usage::Static)
		{
			return nullptr;
		}

		d3d_map = (discard) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_READ_WRITE;

		hr = RenderContext::get_context()->Map(m_buffer, NULL, d3d_map, NULL, &mapped_subsource);
		CHECK_HR(hr);

		return mapped_subsource.pData;
	}

	void GpuBuffer::unmap()
	{
		RenderContext::get_context()->Unmap(m_buffer, NULL);
	}

	void GpuBuffer::update(const void* data, size_t size, size_t offset)
	{
		if (m_usage == Usage::Static)
		{
			D3D11_BOX dst_box;
			D3D11_BOX* dst_box_ptr;
			dst_box.left = offset;
			dst_box.right = offset + size;
			dst_box.top = 0;
			dst_box.bottom = 1;
			dst_box.front = 0;
			dst_box.back = 1;
			dst_box_ptr = m_type == Type::Constant ? NULL : &dst_box;
			RenderContext::get_context()->UpdateSubresource(m_buffer, 0, dst_box_ptr, data, 0, 0);
		}
		else if (m_usage == Usage::Dynamic)
		{
			char* mapped_data = (char*)map(true);
			memcpy((void*)(mapped_data + offset), data, size);
			unmap();
		}
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