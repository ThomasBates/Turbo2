
// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 position	: POSITION;
	float3 normal	: NORMAL;
	float2 texcoord	: TEXCOORD0;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 position : SV_POSITION;
	//float4 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
};

