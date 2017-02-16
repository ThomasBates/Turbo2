#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "ITurboApplicationDX12Renderer.h"

#include "TurboApplicationDX12ShaderStructures.h"

namespace Application_DX12
{
	// This sample renderer instantiates a basic rendering pipeline.
	class TurboApplicationDX12Renderer : public ITurboApplicationDX12Renderer
	{
	public:
		TurboApplicationDX12Renderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~TurboApplicationDX12Renderer();

		//  ITurboApplicationDX12Renderer Methods  ---------------------------------------------------------------------------
		void Resize();
		bool RenderStaticScene(std::shared_ptr<ITurboScene> staticScene);
		bool RenderDynamicScene(std::shared_ptr<ITurboScene> dynamicScene);

	private:
		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> _deviceResources;

		std::vector<VertexPositionColor>	_pcVertices;
		std::vector<unsigned short>			_pcIndices;

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

		//std::shared_ptr<ITurboSceneObjectPlacement> _cameraPlacement;

		//	Local Support Methods  --------------------------------------------------------------------------------------------
		void DrawSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
		void EndDraw();
		void UpdateCameraPlacement(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement);
	};
}

