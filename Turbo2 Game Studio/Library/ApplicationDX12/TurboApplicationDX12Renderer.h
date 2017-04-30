#pragma once

#include "..\Common\DeviceResources.h"

#include "ITurboApplicationDX12Renderer.h"
#include "TurboApplicationDX12ShaderStructures.h"

using namespace Windows::System;

namespace Application_DX12
{
	// This sample renderer instantiates a basic rendering pipeline.
	class TurboApplicationDX12Renderer : public ITurboApplicationDX12Renderer
	{
	public:
		TurboApplicationDX12Renderer();
		~TurboApplicationDX12Renderer();

		//  ITurboApplicationDX12Renderer Methods  ---------------------------------------------------------------------------
		virtual void Resize(float width, float height);
		virtual void SetDPI(float logicalDPI);
		virtual void SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation);
		virtual void ValidateDevice();
		virtual bool NeedsReset();

		virtual bool LoadLevelResources(std::shared_ptr<IGameLevel> level);
		virtual bool RenderLevel(std::shared_ptr<IGameLevel> level);

	private:
		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(ProjectionViewModelConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources>	_deviceResources;
		ID3D12Device*							_device;


		std::map<std::shared_ptr<ITurboSceneObject>, Microsoft::WRL::ComPtr<ID3D12Resource>>	_levelVertexTargetResources;
		std::map<std::shared_ptr<ITurboSceneObject>, Microsoft::WRL::ComPtr<ID3D12Resource>>	_levelVertexSourceResources;
		std::map<std::shared_ptr<ITurboSceneObject>, D3D12_VERTEX_BUFFER_VIEW>					_levelVertexBufferViews;

		std::map<std::shared_ptr<ITurboSceneObject>, Microsoft::WRL::ComPtr<ID3D12Resource>>	_levelIndexTargetResources;
		std::map<std::shared_ptr<ITurboSceneObject>, Microsoft::WRL::ComPtr<ID3D12Resource>>	_levelIndexSourceResources;
		std::map<std::shared_ptr<ITurboSceneObject>, D3D12_INDEX_BUFFER_VIEW>					_levelIndexBufferViews;
		
		std::map<std::shared_ptr<ITurboSceneObject>, int>										_levelSceneObjectOffsets;
		int																						_levelSceneObjectCount;

		std::map<std::string, Microsoft::WRL::ComPtr<ID3D12Resource>>							_levelTextureTargetResources;
		std::map<std::string, Microsoft::WRL::ComPtr<ID3D12Resource>>							_levelTextureSourceResources;
		
		std::map<std::string, int>																_levelTextureOffsets;
		int																						_levelTextureCount;


		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	_commandList;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>			_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>			_pipelineState;
		
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		_cbvSrvDescriptorHeap;
		UINT												_cbvSrvDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		_samplerDescriptorHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource>				_constantBufferTargetResource;
		ProjectionViewModelConstantBuffer*					_constantBufferMappedResource;
		ProjectionViewModelConstantBuffer					m_constantBufferData;


		D3D12_RECT											m_scissorRect;

		// Variables used with the rendering loop.

		const UINT64 TextureWidth = 256U;
		const UINT64 TextureHeight = 256U;
		const UINT64 TexturePixelSize = 4U;


		//	Local Support Methods  --------------------------------------------------------------------------------------------
		void CreateRootSignature();
		void CreatePipelineStateObject();
		void CreateCommandList();
		void ResetCommandList();
		void ExecuteCommandList();

		void CreateLevelVertexResources(std::shared_ptr<IGameLevel> level);
		void LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject);
		void LoadVertexData(std::shared_ptr<ITurboSceneObjectMesh> mesh, std::vector<ShaderVertex> *vertexList, std::vector<unsigned short> *indexList);

		void CreateLevelTextureResources(std::shared_ptr<IGameLevel> level);
		void LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject);
		void LoadTextureData(std::string textureName, D3D12_RESOURCE_DESC *textureResourceDesc, std::vector<unsigned char> *textureData);

		void CreateConstantBufferResources();
		void CreateCBVSRVDescriptorHeap();
		void CreateCBVDescriptors();
		void CreateSRVDescriptors();
		void CreateSamplerDescriptorHeap();
		void CreateSamplerDescriptor();

		void UpdateProjectionMatrix();
		void UpdateViewMatrix(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement);
		void InitializeRendering();
		void PopulateCommandList(std::shared_ptr<IGameLevel> level);
		void PopulateCommandList(std::shared_ptr<ITurboSceneObject> sceneObject);
		void FinalizeRendering();

	};
}

