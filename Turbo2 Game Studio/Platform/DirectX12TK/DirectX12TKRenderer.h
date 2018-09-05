#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>
#include <ITurboScene.h>
#include <ITurboSceneMesh.h>

#include <DirectX12TKDeviceResources.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::System;

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

namespace Turbo 
{
	namespace Platform 
	{
		namespace DirectX12TK
		{
			// This sample renderer instantiates a basic rendering pipeline.
			class DirectX12TKRenderer : public ITurboGameRenderer, public IDeviceNotify
			{
			public:
				DirectX12TKRenderer(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService);
				~DirectX12TKRenderer();

				//	ITurboGameRenderer Methods -------------------------------------------------------------------------
				virtual void UpdateDisplayInformation();
				virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
				virtual bool RenderScene(std::shared_ptr<ITurboScene> scene);

				//	IDeviceNotify Methods ------------------------------------------------------------------------------
				virtual void OnDeviceLost();
				virtual void OnDeviceRestored();


			private:
				// Constant buffers must be 256-byte aligned.
				//static const UINT c_alignedConstantBufferSize = (sizeof(SceneConstantBuffer) + 255) & ~255;

				std::shared_ptr<ITurboDebug>							_debug = nullptr;
				std::shared_ptr<ITurboGameIOService>					_ioService = nullptr;

				// Cached pointer to device resources.
				std::unique_ptr<DeviceResources>						_deviceResources = nullptr;
				ID3D12Device*											_device = NULL;

				//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
				std::unique_ptr<GraphicsMemory>							_graphicsMemory;

				//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
				std::unique_ptr<DescriptorHeap>							_descriptorHeap;
				std::unique_ptr<CommonStates>							_commonStates;

				ResourceUploadBatch*									_resourceUploadBatch;

				std::unique_ptr<SpriteBatch>							_spriteBatch;
				std::unique_ptr<SpriteBatch>							_textBatch;
				RECT													_fullscreenRect;

				size_t													_descriptorCount;

				std::vector<std::wstring>								_sceneTextureNames;
				std::map<std::wstring, ComPtr<ID3D12Resource>>			_sceneTextureResources;
				std::map<std::wstring, size_t>							_sceneTextureDescriptors;

				std::vector<std::wstring>								_sceneFontNames;
				std::map<std::wstring, std::shared_ptr<SpriteFont>>		_sceneFontResources;
				std::map<std::wstring, size_t>							_sceneFontDescriptors;

				std::unique_ptr<BasicEffect>							_lineEffect;
				std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	_lineBatch;

				SimpleMath::Matrix										m_world;
				SimpleMath::Matrix										m_view;
				SimpleMath::Matrix										m_proj;

				std::map<std::shared_ptr<ITurboSceneMesh>,
					std::shared_ptr<GeometricPrimitive>>				_meshPrimitiveShapes;
				std::map<std::shared_ptr<ITurboSceneObject>,
					std::shared_ptr<GeometricPrimitive>>				_primitiveShapes;
				std::map<std::shared_ptr<ITurboSceneObject>,
					std::shared_ptr<BasicEffect>>						_primitiveEffects;



				//std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneVertexTargetResources;
				//std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneVertexSourceResources;
				//std::map<std::shared_ptr<ITurboSceneMesh>, D3D12_VERTEX_BUFFER_VIEW>	_sceneVertexBufferViews;

				//std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneIndexTargetResources;
				//std::map<std::shared_ptr<ITurboSceneMesh>, ComPtr<ID3D12Resource>>		_sceneIndexSourceResources;
				//std::map<std::shared_ptr<ITurboSceneMesh>, D3D12_INDEX_BUFFER_VIEW>		_sceneIndexBufferViews;

				//std::map<std::string, ComPtr<ID3D12Resource>>							_sceneTextureTargetResources;
				//std::map<std::string, ComPtr<ID3D12Resource>>							_sceneTextureSourceResources;

				//std::map<std::shared_ptr<ITurboSceneObject>, UINT>						_sceneObjectOffsets;
				//UINT																	_sceneObjectCount;
				//std::map<std::shared_ptr<ITurboSceneMesh>, UINT>						_sceneObjectMeshOffsets;
				//UINT																	_sceneObjectMeshCount;
				//std::map<std::string, UINT>												_sceneObjectTextureOffsets;
				//UINT																	_sceneObjectTextureCount;


				//ComPtr<ID3D12GraphicsCommandList>	_commandList = nullptr;
				//std::unique_ptr<DescriptorHeap>							_descriptorHeap;
				//std::unique_ptr<CommonStates>							_commonStates;


				//ComPtr<ID3D12RootSignature>			_rootSignature = nullptr;
				//ComPtr<ID3D12PipelineState>			_pipelineState = nullptr;

				//ComPtr<ID3D12DescriptorHeap>		_cbvSrvDescriptorHeap = nullptr;
				//UINT								_cbvSrvDescriptorSize;
				//ComPtr<ID3D12DescriptorHeap>		_samplerDescriptorHeap = nullptr;

				//ComPtr<ID3D12Resource>				_constantBufferTargetResource = nullptr;
				//SceneObjectConstantBuffer*			_constantBufferMappedResource = NULL;
				//SceneLightConstantBuffer*			_constantBufferLightData = NULL;
				//SceneConstantBuffer*				_constantBufferData = NULL;


				//D3D12_RECT							_scissorRect;

				// Variables used with the rendering loop.

				//const UINT64 TextureWidth = 256U;
				//const UINT64 TextureHeight = 256U;
				//const UINT64 TexturePixelSize = 4U;

				ID3D12GraphicsCommandList*								_commandList;

				//	Local Support Methods --------------------------------------------------------------------------------------
				void ReleaseSceneResources();

				void GetSceneResourceNames(std::shared_ptr<ITurboScene> scene);
				void GetSceneSpriteTextureName(std::shared_ptr<ITurboSceneSprite> sceneSprite);
				void GetSceneObjectTextureNames(std::shared_ptr<ITurboSceneObject> sceneObject);
				void GetSceneObjectTextureName(std::shared_ptr<ITurboSceneObject> sceneObject);
				void GetSceneTextFontName(std::shared_ptr<ITurboSceneText> sceneText);

				void CreateDescriptorHeap();
				void CreateTextureResources();
				void CreateFontResources();
				void CreateSpriteBatch(std::shared_ptr<ITurboScene> scene);
				void CreateTextBatch(std::shared_ptr<ITurboScene> scene);

				void CreateSceneResources(std::shared_ptr<ITurboScene> scene);
				void CreateSceneObjectResource(std::shared_ptr<ITurboSceneObject> sceneObject);
				void CreateSceneObjectMeshResource(std::shared_ptr<ITurboSceneObject> sceneObject);
				void CreateSceneObjectModelResource(std::shared_ptr<ITurboSceneObject> sceneObject);
				void CreateSceneObjectPrimitiveResource(std::shared_ptr<ITurboSceneObject> sceneObject);

				std::wstring GetTextureFileName(std::wstring textureName);
				std::wstring GetFontFileName(std::wstring fontName);

				void Clear();

				void UpdateProjectionMatrix();
				void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack);
				void InitializeRender();
				void RenderSprites(std::shared_ptr<ITurboScene> scene);
				void RenderText(std::shared_ptr<ITurboScene> scene);
				void RenderPrimitives(std::shared_ptr<ITurboScene> scene);
				void RenderModels(std::shared_ptr<ITurboScene> scene);
				void RenderMeshes(std::shared_ptr<ITurboScene> scene);
				void RenderSceneObjectMeshes(std::shared_ptr<ITurboScene> scene, std::shared_ptr<ITurboSceneObject> sceneObject);
				void RenderSceneObjectMesh(std::shared_ptr<ITurboScene> scene, std::shared_ptr<ITurboSceneObject> sceneObject);
				void FinalizeRender();
			};
		}
	}
}
