
#include <ShaderStructures.hlsli>

//	HLSL Introduction (including Diffuse Color, Diffuse and Specular, Draw Texture)
//	http://www.neatware.com/lbstudio/web/hlsl.html

// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer SceneConstantBuffer : register(b0)
{
	matrix sceneProjection;
	matrix sceneView;
	matrix sceneViewInverseTranspose;
	float4 sceneLightVector;
	float4 sceneCameraVector;
};

cbuffer SceneObjectConstantBuffer : register(b1)
{
	matrix sceneObjectModel;
	matrix sceneObjectModelInverseTranspose;
};


// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 position = float4(input.position, 1.0f);

	// Transform the vertex position into projected space.
	position = mul(position, sceneObjectModel);
	position = mul(position, sceneView);
	output.position = mul(position, sceneProjection);

	float litArea = 0.5;

	float distance = length(position);
	float power = 1.0;
	if (distance > 1.0 + litArea)
	{
		power = 1.0 / (distance - litArea);
		power = pow(power, 3);
	}

	output.color = float4(power, power, power, 1.0f);

	// Pass the texture coordinates through without modification.
	output.texcoord = input.texcoord;

	return output;
}

/*

float4 normal = float4(input.normal, 0.0f);
normal = mul(normal, sceneObjectModel);
normal = mul(normal, sceneView);
normal = normalize(normal);

float4 light = sceneLightVector;
light = normalize(position);

float4 eye = sceneCameraVector;
eye = normalize(position);

//float4 light = normalize(sceneLightVector);
//float4 eye = sceneCameraVector;
float4 vhalf = normalize(light + eye);
//float4 vhalf = normalize(position);

float diffuse = dot(normal, -light);
float specular = dot(normal, vhalf);
specular = pow(specular, 32);

float4 diffuseMaterial = float4(0.5, 0.5, 1.0, 1.0);
float4 specularMaterial = float4(0.5, 0.5, 1.0, 1.0);

//output.color = diffuse * diffuseMaterial + specular * specularMaterial;
//output.color = diffuse + specular;
//output.color = diffuse;

// Pass the texture coordinates through without modification.
output.texcoord = input.texcoord;

//output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
//output.color = normal * 0.25 + float4(0.5f, 0.5f, 0.5f, 1.0f);
//abs(normal.x), abs(normal.y), abs(normal.z), 1.0f);

*/