#include "shadertext.h"

const char* SIMPLE_SHADER_VS = R"(

struct VertexInputType
{
    float3 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

PixelInputType vs_main(VertexInputType input)
{
    PixelInputType output;
    
	output.position = float4(input.position.x, input.position.y, 0.0, 1.0);

    return output;
}
)";

const char* SIMPLE_SHADER_PS = R"(

cbuffer ConstantInfo : register( b0 )
{
	float4 color;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
};

float4 ps_main(PixelInputType input) : SV_TARGET
{
    return color;
}
)";
