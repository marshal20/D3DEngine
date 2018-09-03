#include "shadertext.h"

const char* SIMPLE_SHADER_VS = R"(

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

const char* SIMPLE_SHADER_PS = R"(

cbuffer ConstantInfo : register( b0 )
{
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
    float4 color = shaderTexture.Sample(SampleType, input.coord);
    return lerp(color, kcolor, 0.5);
}
)";
