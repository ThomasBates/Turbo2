#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>
#include <ITurboScene.h>
#include <ITurboSceneMesh.h>

#include <DirectX12DeviceResources.h>
#include <DirectX12ShaderStructures.h>

using namespace Microsoft::WRL;
using namespace Windows::System;

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

namespace Turbo 
{
	namespace Platform 
	{
		namespace DirectX12
		{
			// This sample renderer instantiates a basic rendering pipeline.
			class DirectX12Renderer : public ITurboGameRenderer
			{
			public:
				DirectX12Renderer(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService);
				~DirectX12Renderer();

				//	ITurboGameRenderer Methods -------------------------------------------------------------------------
				virtual void UpdateDisplayInformation();
				virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
				virtual bool RenderScene(std::shared_ptr<ITurboScene> scene);

			private:
				// Constant buffers must be 256-byte aligned.
				static const UINT c_alignedConstantBufferSize = (sizeof(SceneConstantBuffer) + 255) & ~255;

				std::shared_ptr<ITurboDebug>				_debug = nullptr;
				std::shared_ptr<ITurboGameIOService>		_ioService = nullptr;

				// Cached pointer to device resources.
				std::shared_ptr<DeviceResources>			_deviceResources = nullptr;
				ID3D12Device*								_device = NULL;


				std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneVertexTargetResources;
				std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneVertexSourceResources;
				std::map<std::shared_ptr<ITurboSceneMesh>, D3D12_VERTEX_BUFFER_VIEW>	_sceneVertexBufferViews;

				std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneIndexTargetResources;
				std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneIndexSourceResources;
				std::map<std::shared_ptr<ITurboSceneMesh>, D3D12_INDEX_BUFFER_VIEW>		_sceneIndexBufferViews;

				std::map<std::string, ComPtr<ID3D12Resource>>							_sceneTextureTargetResources;
				std::map<std::string, ComPtr<ID3D12Resource>>							_sceneTextureSourceResources;

				std::map<std::shared_ptr<ITurboSceneObject>, UINT>						_sceneObjectOffsets;
				UINT																	_sceneObjectCount;
				std::map<std::shared_ptr<ITurboSceneMesh>, UINT>						_sceneObjectMeshOffsets;
				UINT																	_sceneObjectMeshCount;
				std::map<std::string, UINT>												_sceneObjectTextureOffsets;
				UINT																	_sceneObjectTextureCount;


				ComPtr<ID3D12GraphicsCommandList>	_commandList = nullptr;
				ComPtr<ID3D12RootSignature>			_rootSignature = nullptr;
				ComPtr<ID3D12PipelineState>			_pipelineState = nullptr;

				ComPtr<ID3D12DescriptorHeap>		_cbvSrvDescriptorHeap = nullptr;
				UINT								_cbvSrvDescriptorSize;
				ComPtr<ID3D12DescriptorHeap>		_samplerDescriptorHeap = nullptr;

				ComPtr<ID3D12Resource>				_constantBufferTargetResource = nullptr;
				SceneObjectConstantBuffer*			_constantBufferMappedResource = NULL;
				SceneLightConstantBuffer*			_constantBufferLightData = NULL;
				SceneConstantBuffer*				_constantBufferData = NULL;


				D3D12_RECT							_scissorRect;

				// Variables used with the rendering loop.

				//const UINT64 TextureWidth = 256U;
				//const UINT64 TextureHeight = 256U;
				//const UINT64 TexturePixelSize = 4U;

				//	Local Support Methods --------------------------------------------------------------------------------------
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
				void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack);
				void InitializeRendering();
				void PopulateCommandList(std::shared_ptr<ITurboScene> scene);
				void PopulateCommandList(std::shared_ptr<ITurboSceneObject> sceneObject);
				void FinalizeRendering();
			};
		}
	}
}
