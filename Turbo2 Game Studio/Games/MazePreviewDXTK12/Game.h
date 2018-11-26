//
// Game.h
//

#pragma once

#include <map>
#include <DirectX12TKDeviceResources.h>
#include <Windows10Timer.h>

#include <ITurboScene.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::System;

using namespace Turbo::Scene;
using namespace Turbo::Platform::DirectX12TK;
using namespace Turbo::Platform::Windows10;

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game : public IDeviceNotify
{
public:

    Game();

	void ComposeTestScene();

    // Initialization and management
    void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
    void ValidateDevice();

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(Timer const& timer);
    void Render();
	void RenderSprites(ID3D12GraphicsCommandList *commandList);
	void RenderText(ID3D12GraphicsCommandList *commandList);
	void RenderTriangles(ID3D12GraphicsCommandList *commandList);
	void RenderLines(ID3D12GraphicsCommandList *commandList);
	void RenderPrimitives(ID3D12GraphicsCommandList *commandList);
	void RenderModels(ID3D12GraphicsCommandList *commandList);
	void RenderMeshes(ID3D12GraphicsCommandList *commandList);

    void Clear();

	std::wstring GetTextureFileName(std::wstring textureName);
	std::wstring GetFontFileName(std::wstring fontName);

	void CreateDeviceDependentResources();
	void CreateSpriteTextureResources(ResourceUploadBatch *resourceUploadBatch);
	void CreateSpriteBatch(ResourceUploadBatch *resourceUploadBatch);
	void CreateTextFontResources(ResourceUploadBatch *resourceUploadBatch);
	void CreateTextBatch(ResourceUploadBatch *resourceUploadBatch);
	void CreateTriangleBatch();
	void CreateLineBatch();
	void CreateSceneTextureResources(ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneObjectTextureResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneResources(ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneObjectResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneObjectMeshResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneObjectModelResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch);
	void CreateSceneObjectPrimitiveResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch);
	
	void CreateTextureResource(std::shared_ptr<ITurboSceneTexture> texture, ResourceUploadBatch *resourceUploadBatch);
	void CreateFontResource(std::shared_ptr<ITurboSceneFont> font, ResourceUploadBatch *resourceUploadBatch);

    void CreateWindowSizeDependentResources();

	std::shared_ptr<ITurboScene> _scene;

    // Device resources.
    std::unique_ptr<DeviceResources>					_deviceResources;
	ID3D12Device											*_device;

    // Rendering loop timer.
    Timer									         m_timer;

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	std::unique_ptr<GraphicsMemory>							_graphicsMemory;

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
	std::unique_ptr<DescriptorHeap>							_descriptorHeap;

	std::unique_ptr<CommonStates>							_spriteStates;
	std::unique_ptr<SpriteBatch>							_spriteBatch;
	std::unique_ptr<SpriteBatch>							_textBatch;
	RECT													_fullscreenRect;
	
	size_t													_descriptorCount;

	std::map<std::wstring, ComPtr<ID3D12Resource>>			_sceneTextureResources;
	std::map<std::wstring, size_t>							_sceneTextureDescriptors;

	std::map<std::wstring, 
			std::shared_ptr<SpriteFont>>					_sceneFontResources;
	std::map<std::wstring, size_t>							_sceneFontDescriptors;

	std::unique_ptr<BasicEffect>							_triangleEffect;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	_triangleBatch;

	std::unique_ptr<BasicEffect>							_lineEffect;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>>	_lineBatch;

	SimpleMath::Matrix										m_world;
	SimpleMath::Matrix										m_view;
	SimpleMath::Matrix										m_proj;

	std::unique_ptr<CommonStates>							_primitiveStates;
	std::map<std::shared_ptr<ITurboSceneObject>,
			 std::shared_ptr<GeometricPrimitive>>			_primitiveShapes;
	std::map<std::shared_ptr<ITurboSceneObject>,
			 std::shared_ptr<BasicEffect>>					_primitiveEffects;
};