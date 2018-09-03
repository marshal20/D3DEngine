#include <CoreEngine/rendersystem/renderer2d.h>

#include <CoreEngine/graphics/rendercontext.h>
#include <CoreEngine/graphics/rastrizerstate.h>
#include <CoreEngine/graphics/texture2d.h>
#include <CoreEngine/graphics/gpubuffer.h>
#include <CoreEngine/graphics/layout.h>
#include <CoreEngine/graphics/shader.h>
#include <CoreEngine/rendersystem/sprite.h>
#include "../utils/safemem.h"

const char* sprite_renderer_vertex_shader =
R"(
struct VertexInputType
{
    float2 position : POSITION;
	float2 coord : TEXCOORDS;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 coord : TEXCOORDS;
};

PixelInputType vs_main(VertexInputType input)
{
    PixelInputType output;
    
	output.position = float4(input.position.x, input.position.y, 0.0, 1.0);
	output.coord = input.coord;

    return output;
}
)";

const char* sprite_renderer_pixel_shader =
R"(
cbuffer ConstantInfo : register( b0 )
{
	bool ktextured;
	float4 kcolor;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 coord : TEXCOORDS;
};

Texture2D shaderTexture : register( t0 );
SamplerState SampleType : register( s0 );

float4 ps_main(PixelInputType input) : SV_TARGET
{
    float4 color;
	if(ktextured) {
		color = shaderTexture.Sample(SampleType, input.coord);
	}
	else {
		color = kcolor;
	}
    return kcolor;
}
)";

namespace ce
{
	struct Renderer2D::Vertex
	{
		math::Vec2<float> position;
		math::Vec2<float> texcoords;
	};

	struct Renderer2D::Batch
	{
		size_t start_index;
		size_t count;
		math::Vec4<float> color;
		Texture2D* texture;
	};
	
	struct ConstantBuffer
	{
		bool ktextured;
		math::Vec4<float> kcolor;
	};

	Renderer2D::Renderer2D()
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	void Renderer2D::init()
	{
		m_rasterizer_state = new RasterizerState;
		m_vertex_shader = new Shader;
		m_pixel_shader = new Shader;
		m_layout = new Layout;
		m_vertex_buffer = new GpuBuffer;
		m_constant_buffer = new GpuBuffer;

		m_vertex_array.resize(MAX_SPRITES);

		m_rasterizer_state->init(ce::RasterizerState::Cull::Back,
			ce::RasterizerState::Fill::Solid, ce::RasterizerState::Rotation::CCW);

		m_vertex_shader->load_from_memory(sprite_renderer_vertex_shader, "vs_main", ce::Shader::Type::Vertex);
		m_pixel_shader->load_from_memory(sprite_renderer_pixel_shader, "ps_main", ce::Shader::Type::Pixel);

		m_layout->push({ "POSITION", ce::Layout::Element::Type::FLOAT, 2 });
		m_layout->push({ "TEXCOORDS", ce::Layout::Element::Type::FLOAT, 2 });
		m_layout->create();

		m_vertex_buffer->init(MAX_SPRITES * sizeof(Vertex),
			ce::GpuBuffer::Type::Vertex, ce::GpuBuffer::Usage::Dynamic);

		m_constant_buffer->init(sizeof(ConstantBuffer),
			ce::GpuBuffer::Type::Vertex, ce::GpuBuffer::Usage::Dynamic);
	}

	void Renderer2D::cleanup()
	{
		m_rasterizer_state->cleanup();
		m_vertex_shader->cleanup();
		m_pixel_shader->cleanup();
		m_layout->cleanup();
		m_vertex_buffer->cleanup();
		m_constant_buffer->cleanup();

		SAFE_FREE(m_rasterizer_state);
		SAFE_FREE(m_vertex_shader);
		SAFE_FREE(m_pixel_shader);
		SAFE_FREE(m_layout);
		SAFE_FREE(m_vertex_buffer);
		SAFE_FREE(m_constant_buffer);
	}

	void Renderer2D::draw(const Sprite* sprite)
	{
		float top, left, bottom, right;
		Batch cur_batch;

		top = sprite->position.y;
		left = sprite->position.x;
		bottom = top + sprite->size.y;
		right = left + sprite->size.x;

		m_vertex_array[m_cur_index] = { { left, top },{ +0.0f, +0.0f } };
		m_cur_index++;
		m_vertex_array[m_cur_index] = { { left, bottom },{ +0.0f, +1.0f } };
		m_cur_index++;
		m_vertex_array[m_cur_index] = { { right, top },{ +1.0f, +0.0f } };
		m_cur_index++;
		m_vertex_array[m_cur_index] = { { left, bottom },{ +0.0f, +1.0f } };
		m_cur_index++;
		m_vertex_array[m_cur_index] = { { right, bottom },{ +1.0f, +1.0f } };
		m_cur_index++;
		m_vertex_array[m_cur_index] = { { right, top },{ +1.0f, +0.0f } };
		m_cur_index++;

		m_batches.push_back({ m_cur_index - 6, 6, sprite->color, sprite->texture });
	}

	void Renderer2D::update()
	{
		Vertex* data = (Vertex*)m_vertex_buffer->map(true);
		memcpy(data, &m_vertex_array[0], sizeof(Vertex) * m_cur_index);
		m_vertex_buffer->unmap();
	}

	void Renderer2D::render()
	{
		ConstantBuffer constant_buffer;

		m_vertex_shader->use();
		m_pixel_shader->use();
		m_layout->use();
		RenderContext::set_vertex_buffer(0, m_vertex_buffer, sizeof(Vertex), 0);
		for (const Batch& batch : m_batches)
		{
			constant_buffer.ktextured = (batch.texture != nullptr);
			constant_buffer.kcolor = batch.color;
			m_constant_buffer->update(&constant_buffer);
			RenderContext::set_constant_buffer(ce::Shader::Type::Pixel, 0, m_constant_buffer);
			if (batch.texture)
			{
				batch.texture->use(0);
			}
			RenderContext::set_primitive_topology(ce::PrimitiveTopology::TriangleList);
			RenderContext::draw(batch.count, batch.start_index);
		}
	}

	void Renderer2D::clear()
	{
		m_cur_index = 0;
		m_batches.clear();
	}
}