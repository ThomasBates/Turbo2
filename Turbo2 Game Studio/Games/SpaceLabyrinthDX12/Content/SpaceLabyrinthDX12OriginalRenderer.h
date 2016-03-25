#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "ISpaceLabyrinthRenderer.h"

namespace SpaceLabyrinthDX12
{
	// This sample renderer instantiates a basic rendering pipeline.
	class SpaceLabyrinthDX12OriginalRenderer : public ISpaceLabyrinthRenderer
	{
	public:
		SpaceLabyrinthDX12OriginalRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~SpaceLabyrinthDX12OriginalRenderer();

		void Initialize();
		void Resize();
		void BeginDraw();
		void EndDraw();
		void Update(double elapsedSeconds);
		bool Render();
		void Finalize();

		void DrawCorner(MazeObject *corner);
		void DrawEdge(MazeObject *edge);
		void DrawWall(MazeObject *wall);

	private:
		void SaveState();
		
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() { return m_tracking; }

		void LoadState();
		void Rotate(float radians);

	private:
		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::vector<VertexPositionColor>	m_cubeVertices;
		std::vector<unsigned short>			m_cubeIndices;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_commandList;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cbvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_constantBuffer;
		ModelViewProjectionConstantBuffer					m_constantBufferData;
		UINT8*												m_mappedConstantBuffer;
		UINT												m_cbvDescriptorSize;
		D3D12_RECT											m_scissorRect;
		std::vector<byte>									m_vertexShader;
		std::vector<byte>									m_pixelShader;
		D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_radiansPerSecond;
		float	m_angle;
		bool	m_tracking;
	};
}

