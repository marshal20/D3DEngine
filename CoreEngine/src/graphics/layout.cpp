#include <CoreEngine/graphics/layout.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <CoreEngine/graphics/rendercontext.h>
#include "../utils/safemem.h"
#include "../utils/callcheck.h"

namespace ce
{
	using ElementDescList = std::vector<D3D11_INPUT_ELEMENT_DESC>;
	using ElementList = std::vector<Layout::Element>;
	void create_elements_desc(ElementDescList& elements_desc, const ElementList& elements);
	void get_dummy_shader_string(std::string& out, const ElementList& elements_list);

	Layout::Layout()
	{
	}

	Layout::~Layout()
	{
	}

	void Layout::create()
	{
		HRESULT hr;
		ElementDescList elemnts_desc;
		std::string dummy_shader_text;
		ID3D10Blob* dummy_shader_blob;
		ID3D10Blob* dummy_message_blob;

		get_dummy_shader_string(dummy_shader_text, m_list);
		hr = D3DCompile(dummy_shader_text.c_str(), dummy_shader_text.size(), NULL, NULL, NULL,
			"dummy_main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &dummy_shader_blob, &dummy_message_blob);
		CHECK_HR(hr);
		// TODO: Add proper message reporting.
		if (FAILED(hr))
		{
			printf("%s\n", (char*)dummy_message_blob->GetBufferPointer());
		}

		SAFE_RELEASE(dummy_message_blob);

		create_elements_desc(elemnts_desc, m_list);

		hr = RenderContext::get_device()->CreateInputLayout(&elemnts_desc[0], elemnts_desc.size(),
			dummy_shader_blob->GetBufferPointer(), dummy_shader_blob->GetBufferSize(), &m_layout);
		CHECK_HR(hr);

		SAFE_RELEASE(dummy_shader_blob);
	}

	void Layout::cleanup()
	{
		SAFE_RELEASE(m_layout);
	}

	void Layout::push(const Element& element)
	{
		m_list.push_back(element);
	}

	void Layout::use()
	{
		RenderContext::get_context()->IASetInputLayout(m_layout);
	}

	// HELPER FUNCTIONS

	DXGI_FORMAT get_element_format(const Layout::Element& element)
	{
		switch (element.type)
		{
		case Layout::Element::Type::FLOAT:
			switch (element.count)
			{
			case 1: return DXGI_FORMAT_R32_FLOAT;
			case 2: return DXGI_FORMAT_R32G32_FLOAT;
			case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default: return  DXGI_FORMAT_R32_TYPELESS;
			}

		case Layout::Element::Type::INT:
			switch (element.count)
			{
			case 1: return DXGI_FORMAT_R32_SINT;
			case 2: return DXGI_FORMAT_R32G32_SINT;
			case 3: return DXGI_FORMAT_R32G32B32_SINT;
			case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
			default: return  DXGI_FORMAT_R32_TYPELESS;
			}
		default: return  DXGI_FORMAT_R32_TYPELESS;
		}
	}

	void create_elements_desc(ElementDescList& elements_desc, const ElementList& elements)
	{
		elements_desc.clear();
		elements_desc.reserve(elements.size());

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			D3D11_INPUT_ELEMENT_DESC curInputElement;
			const Layout::Element& element = elements[i];

			curInputElement.SemanticName = element.name.c_str();
			curInputElement.SemanticIndex = 0;
			curInputElement.Format = get_element_format(element);
			curInputElement.InputSlot = 0;
			curInputElement.AlignedByteOffset = (i == 0) ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
			curInputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			curInputElement.InstanceDataStepRate = 0;

			elements_desc.push_back(curInputElement);
		}
	}

	void get_dummy_shader_string(std::string& out, const ElementList& elements_list)
	{
		const char* dummy_shader_start = "struct VertexInputType { ";
		const char* dummy_shader_end = "}; float4 dummy_main(VertexInputType input) : SV_POSITION { return float4(0.0, 0.0, 0.0, 0.0); }";

		out = dummy_shader_start;
		// Add all elements for VertexInputType struct. Ex: float3 input0 : POSITION;
		for (unsigned int i = 0; i < elements_list.size(); i++)
		{
			const Layout::Element& element = elements_list[i];
			std::string element_type;
			std::string input_line;

			switch (element.type)
			{
			case Layout::Element::Type::FLOAT: element_type = "float"; break;
			case Layout::Element::Type::INT: element_type = "int"; break;
			default: break;
			}
			element_type += std::to_string(element.count);
			input_line = element_type + " input" + std::to_string(i) + " : " + element.name + "; ";

			out += input_line;
		}
		out += dummy_shader_end;
	}
}