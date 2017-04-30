
#include <ShaderStructures.hlsli>

//
//// Per-pixel color data passed through the pixel shader.
//struct PixelShaderInput
//{
//	float4 pos : SV_POSITION;
//	float2 textureuv : TEXCOORD0;
//};
//

Texture2D		g_texture : register(t0);
SamplerState	g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 color = g_texture.Sample(g_sampler, input.texcoord).rgb;

	return float4(color, 1.0f);
}
