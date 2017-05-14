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

		virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
		virtual bool RenderScene(std::shared_ptr<ITurboScene> scene);

	private:
		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(SceneConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources>				_deviceResources;
		ID3D12Device*										_device;


		std::map<std::shared_ptr<ITurboSceneMesh>,	Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneVertexTargetResources;
		std::map<std::shared_ptr<ITurboSceneMesh>,	Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneVertexSourceResources;
		std::map<std::shared_ptr<ITurboSceneMesh>,	D3D12_VERTEX_BUFFER_VIEW>					_sceneVertexBufferViews;

		std::map<std::shared_ptr<ITurboSceneMesh>,	Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneIndexTargetResources;
		std::map<std::shared_ptr<ITurboSceneMesh>,	Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneIndexSourceResources;
		std::map<std::shared_ptr<ITurboSceneMesh>,	D3D12_INDEX_BUFFER_VIEW>					_sceneIndexBufferViews;

		std::map<std::string,						Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneTextureTargetResources;
		std::map<std::string,						Microsoft::WRL::ComPtr<ID3D12Resource>>		_sceneTextureSourceResources;

		std::map<std::shared_ptr<ITurboSceneObject>, int>										_sceneObjectOffsets;
		int																						_sceneObjectCount;
		std::map<std::shared_ptr<ITurboSceneMesh>,	int>										_sceneObjectMeshOffsets;
		int																						_sceneObjectMeshCount;
		std::map<std::string,						int>										_sceneObjectTextureOffsets;
		int																						_sceneObjectTextureCount;


		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	_commandList;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>			_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>			_pipelineState;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		_cbvSrvDescriptorHeap;
		UINT												_cbvSrvDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		_samplerDescriptorHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource>				_constantBufferTargetResource;
		SceneObjectConstantBuffer*							_constantBufferMappedResource;
		SceneConstantBuffer*								_constantBufferData;


		D3D12_RECT											_scissorRect;

		// Variables used with the rendering loop.

		const UINT64 TextureWidth = 256U;
		const UINT64 TextureHeight = 256U;
		const UINT64 TexturePixelSize = 4U;

		//	Local Support Methods  --------------------------------------------------------------------------------------------
		void ReleaseSceneResources();
		void CreateRootSignature();
		void CreatePipelineStateObject();
		void CreateCommandList();
		void ResetCommandList();
		void ExecuteCommandList();

		void CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene);
		void LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject);
		void LoadVertexData(std::shared_ptr<ITurboSceneMesh> mesh, std::vector<ShaderVertex> *vertexList, std::vector<unsigned short> *indexList);

		void CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene);
		void LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject);
		void LoadTextureData(std::string textureName, D3D12_RESOURCE_DESC *textureResourceDesc, std::vector<unsigned char> *textureData);

		void CreateConstantBufferResources();
		void CreateCBVSRVDescriptorHeap();
		void CreateCBVDescriptors();
		void CreateSRVDescriptors();
		void CreateSamplerDescriptorHeap();
		void CreateSamplerDescriptor();

		void UpdateProjectionMatrix();
		void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement);
		void InitializeRendering();
		void PopulateCommandList(std::shared_ptr<ITurboScene> scene);
		void PopulateCommandList(std::shared_ptr<ITurboSceneObject> sceneObject);
		void FinalizeRendering();

	};
}

