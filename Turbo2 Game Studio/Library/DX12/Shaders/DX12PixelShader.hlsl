
#include <DX12ShaderStructures.hlsli>

Texture2D		g_texture : register(t0);
SamplerState	g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 color = float4(g_texture.Sample(g_sampler, input.texcoord).rgb, 1.0f);

//	return color;					//	Just the texture color
//	return input.color;				//	Just the lighting color
	return color * input.color;		//	Combination of lighting and texture

}
