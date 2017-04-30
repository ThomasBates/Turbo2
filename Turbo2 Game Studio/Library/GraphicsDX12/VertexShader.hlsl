
#include <ShaderStructures.hlsli>

// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer SceneConstantBuffer : register(b0)
{
	matrix sceneProjection;
	matrix sceneView;
	matrix sceneModel;
};
//
//// Per-vertex data used as input to the vertex shader.
//struct VertexShaderInput
//{
//	float3 pos : POSITION;
//	//float3 normal : NORMAL;
//	float2 textureuv : TEXCOORD0;
//};
//
//// Per-pixel color data passed through the pixel shader.
//struct PixelShaderInput
//{
//	float4 pos : SV_POSITION;
//	float2 textureuv : TEXCOORD0;
//};


/*
cbuffer SceneObjectConstantBuffer : register(b1)
{
	matrix sceneObjectProjection;
	matrix sceneObjectView;
	matrix sceneObjectModel;
};
*/

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.position, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, sceneModel);
	pos = mul(pos, sceneView);
	pos = mul(pos, sceneProjection);
	output.position = pos;

	// Pass the color through without modification.
	//output.normal = float4(input.normal, 0.0f);
	output.texcoord = input.texcoord;

	return output;
}
