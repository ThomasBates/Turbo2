
#include <ShaderStructures.hlsli>

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	//return float4(input.color, 1.0f);

	float3 mat = g_texture.Sample(g_sampler, input.textureuv).rgb;

	return float4(mat, 1.0f);
}
