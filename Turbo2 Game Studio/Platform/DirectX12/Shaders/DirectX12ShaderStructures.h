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
				DirectX::XMFLOAT4 Light;
				DirectX::XMFLOAT4 Camera;
				DirectX::XMFLOAT4 __filler1;
				DirectX::XMFLOAT4 __filler2;
			};

			// Constant buffer used to send model matrices to the vertex shader.
			struct SceneObjectConstantBuffer
			{
				DirectX::XMFLOAT4X4 Model;
				DirectX::XMFLOAT4X4 ModelInverseTranspose;
				DirectX::XMFLOAT4X4 __filler1;
				DirectX::XMFLOAT4X4 __filler2;
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