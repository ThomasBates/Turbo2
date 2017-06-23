#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Platform
	{
		namespace DirectX12
		{
			// Constant buffer used to send scene matrices to the vertex shader.
			struct SceneConstantBuffer
			{
				DirectX::XMFLOAT4X4 Projection;
				DirectX::XMFLOAT4X4 View;
				DirectX::XMFLOAT4X4 ViewInverseTranspose;

				DirectX::XMFLOAT4 Camera;
				int32 LightCount;

				int32 __filler[11];
			};

			// Constant buffer used to send model matrices to the vertex shader.
			struct SceneObjectConstantBuffer
			{
				DirectX::XMFLOAT4X4 Model;
				DirectX::XMFLOAT4X4 ModelInverseTranspose;

				int32 __filler[32];
			};

			//	Constant buffer used to send lighting model data to the vertex shader.
			struct SceneLightConstantBuffer
			{
				DirectX::XMFLOAT4 LightPosition;
				DirectX::XMFLOAT4 LightDirection;
				DirectX::XMFLOAT4 LightColor;
				int32	LightType;
				float32	LightConstantAttenuation;
				float32	LightLinearAttenuation;
				float32	LightQuadraticAttenuation;
				float32 LightHotSpotAngle;
				float32 LightFallOffAngle;

				int32 __filler[46];
			};

			// Used to send per-vertex data to the vertex shader.
			struct ShaderVertex
			{
				DirectX::XMFLOAT3 Position;
				DirectX::XMFLOAT3 Normal;
				DirectX::XMFLOAT2 Texture;
			};
		}
	}
}