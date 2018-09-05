//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include <TurboScene.h>
#include <TurboSceneFont.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneObject.h>
#include <TurboSceneSprite.h>
#include <TurboSceneText.h>
#include <TurboSceneTexture.h>
#include <Windows10Helpers.h>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()
{
    _deviceResources = std::make_unique<DeviceResources>();
    _deviceResources->RegisterDeviceNotify(this);

	ComposeTestScene();
}

void Game::ComposeTestScene()
{
	_scene = std::shared_ptr<ITurboScene>(new TurboScene());

	std::shared_ptr<ITurboSceneSprite> sprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite());
	sprite->Texture(std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("sunset")));
	//  default: use rectangle (0,0) to (1,1). (full screen)
	_scene->AddSceneSprite(sprite);

	sprite = std::shared_ptr<ITurboSceneSprite>(new TurboSceneSprite());
	sprite->Texture(std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture("cat")));
	sprite->UseRectangle(false);
	sprite->Location(TurboVector2D(0.1f, 0.8f));
	sprite->Origin(TurboVector2D(0.5f, 0.5f));
	sprite->Scale(2.f);
	_scene->AddSceneSprite(sprite);

	std::shared_ptr<ITurboSceneText> text = std::shared_ptr<ITurboSceneText>(new TurboSceneText());
	text->Font(std::shared_ptr<ITurboSceneFont>(new TurboSceneFont("CourierNew_32")));
	text->Text("Hello Kitty!");
	text->Location(TurboVector2D(0.5f, 0.9f));
	text->Origin(TurboVector2D(0.5f, 0.5f));
	text->BorderColor(TurboColor(0.f, 1.f, 0.f));
	text->BorderSize(0.5f);
	_scene->AddSceneText(text);

	text = std::shared_ptr<ITurboSceneText>(new TurboSceneText());
	text->Font(std::shared_ptr<ITurboSceneFont>(new TurboSceneFont("SegoeUI_16_B")));
	text->Text("Look Up Here!");
	text->Location(TurboVector2D(0.1f, 0.05f));
	text->Origin(TurboVector2D(0.5f, 0.5f));
	text->Color(TurboColor(1.f, 1.f, 0.f));
	text->BorderSize(1.0);
	text->BorderColor(TurboColor(1.f, 0.f, 1.f));
	text->Rotation(0.f);
	_scene->AddSceneText(text);

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new TurboSceneObject());
	sceneObject->PrimitiveType(SceneObjectPrimitiveType::Sphere);
	sceneObject->Material(std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("earth")));
	_scene->AddSceneObject(sceneObject);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
    _deviceResources->SetWindow(window, width, height, rotation);

    _deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    _deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

#pragma region Frame Update --------------------------------------------------------------------------------------------

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(Timer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float time = static_cast<float>(timer.GetTotalSeconds());

    // TODO: Add your game logic here.
	//m_world = Matrix::CreateRotationZ(cosf(time) * 2.f);
	m_world = Matrix::CreateRotationY(time);

    PIXEndEvent();
}

#pragma endregion
#pragma region Frame Render --------------------------------------------------------------------------------------------

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    _deviceResources->Prepare();
    Clear();

    auto commandList = _deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here. -------------------------------------

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
//	float time = float(m_timer.GetTotalSeconds());

//	ID3D12DescriptorHeap* heaps[] = { _descriptorHeap->Heap() };
//	ID3D12DescriptorHeap* heaps[] = { _descriptorHeap->Heap(), _spriteStates->Heap() };
	ID3D12DescriptorHeap* heaps[] = { _descriptorHeap->Heap(), _primitiveStates->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	RenderSprites(commandList);
	RenderText(commandList);
	//RenderTriangles(commandList);
	//RenderLines(commandList);
	RenderPrimitives(commandList);
	RenderModels(commandList);
	RenderMeshes(commandList);

	//  ------------------------------------------------------------------------

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");

	_deviceResources->Present();

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	_graphicsMemory->Commit(_deviceResources->GetCommandQueue());

	PIXEndEvent(_deviceResources->GetCommandQueue());
}

void Game::RenderSprites(ID3D12GraphicsCommandList *commandList)
{
	auto sceneSprites = _scene->SceneSprites();

	if (sceneSprites.size() > 0)
	{
		_spriteBatch->Begin(commandList);

		//	Iterate over scene sprites. Call this method recursively.
		for (auto& sceneSprite : sceneSprites)
		{
			TurboColor spriteColor = sceneSprite->Color();
			Color color(spriteColor.R, spriteColor.G, spriteColor.B, spriteColor.A);

			std::shared_ptr<ITurboSceneTexture> texture = sceneSprite->Texture();
			std::wstring textureName = ToWString(texture->Name());

			ComPtr<ID3D12Resource> textureResource = _sceneTextureResources[textureName];
			size_t textureDescriptor = _sceneTextureDescriptors[textureName];

			XMUINT2 textureSize = GetTextureSize(textureResource.Get());

			if (sceneSprite->UseRectangle())
			{
				RECT rect;
				//	Assuming _fullscreenRect top and left are 0.0.
				rect.top = (LONG)(_fullscreenRect.bottom * sceneSprite->Top());
				rect.bottom = (LONG)(_fullscreenRect.bottom * sceneSprite->Bottom());
				rect.left = (LONG)(_fullscreenRect.right * sceneSprite->Left());
				rect.right = (LONG)(_fullscreenRect.right * sceneSprite->Right());

				_spriteBatch->Draw(
					_descriptorHeap->GetGpuHandle(textureDescriptor),
					textureSize,
					rect,
					color);
			}
			else
			{
				DirectX::SimpleMath::Vector2 screenPos;
				screenPos.x = (float)(_fullscreenRect.right * sceneSprite->Location().X);
				screenPos.y = (float)(_fullscreenRect.bottom * sceneSprite->Location().Y);

				DirectX::SimpleMath::Vector2 origin;
				origin.x = float(textureSize.x * sceneSprite->Origin().X);
				origin.y = float(textureSize.y * sceneSprite->Origin().Y);

				_spriteBatch->Draw(
					_descriptorHeap->GetGpuHandle(textureDescriptor),
					textureSize,
					screenPos,
					nullptr,
					color,
					sceneSprite->Rotation(),
					origin,
					sceneSprite->Scale());
			}
		}

		_spriteBatch->End();
	}
}

void Game::RenderText(ID3D12GraphicsCommandList *commandList)
{
	auto sceneTexts = _scene->SceneTexts();

	if (sceneTexts.size() > 0)
	{
		_textBatch->Begin(commandList);

		//	Iterate over scene sprites. Call this method recursively.
		for (auto& sceneText : sceneTexts)
		{
			std::shared_ptr<ITurboSceneFont> font = sceneText->Font();
			std::wstring fontName = ToWString(font->Name());

			std::shared_ptr<DirectX::SpriteFont> fontResource = _sceneFontResources[fontName];
			size_t fontDescriptor = _sceneFontDescriptors[fontName];

			std::wstring text = ToWString(sceneText->Text());

			Vector2 textSize = fontResource->MeasureString(text.c_str());

			Vector2 screenPos;

			screenPos.x = (float)(_fullscreenRect.right * sceneText->Location().X);
			screenPos.y = (float)(_fullscreenRect.bottom * sceneText->Location().Y);

			Vector2 origin;
			origin.x = float(textSize.x * sceneText->Origin().X);
			origin.y = float(textSize.y * sceneText->Origin().Y);

			float borderSize = sceneText->BorderSize();

			if (borderSize > 0)
			{
				TurboColor borderColor = sceneText->BorderColor();
				Color color(borderColor.R, borderColor.G, borderColor.B, borderColor.A);

				fontResource->DrawString(
					_textBatch.get(),
					text.c_str(),
					screenPos + Vector2(borderSize, borderSize),
					color,
					sceneText->Rotation(),
					origin,
					sceneText->Scale());

				fontResource->DrawString(
					_textBatch.get(),
					text.c_str(),
					screenPos + Vector2(-borderSize, borderSize),
					color,
					sceneText->Rotation(),
					origin,
					sceneText->Scale());

				fontResource->DrawString(
					_textBatch.get(),
					text.c_str(),
					screenPos + Vector2(-borderSize, -borderSize),
					color,
					sceneText->Rotation(),
					origin,
					sceneText->Scale());

				fontResource->DrawString(
					_textBatch.get(),
					text.c_str(),
					screenPos + Vector2(borderSize, -borderSize),
					color,
					sceneText->Rotation(),
					origin,
					sceneText->Scale());
			}

			TurboColor spriteColor = sceneText->Color();
			Color color(spriteColor.R, spriteColor.G, spriteColor.B, spriteColor.A);

			fontResource->DrawString(
				_textBatch.get(),
				text.c_str(),
				screenPos,
				color,
				sceneText->Rotation(),
				origin,
				sceneText->Scale());
		}

		_textBatch->End();
	}
}

void Game::RenderTriangles(ID3D12GraphicsCommandList *commandList)
{
	_triangleEffect->Apply(commandList);
	_triangleBatch->Begin(commandList);

	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.2f), Colors::Yellow);

	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Yellow);

	_triangleBatch->DrawTriangle(v1, v2, v3);
	_triangleBatch->End();
}

void Game::RenderLines(ID3D12GraphicsCommandList *commandList)
{
	_lineEffect->SetWorld(m_world);
	_lineEffect->Apply(commandList);
	_lineBatch->Begin(commandList);

	Vector3 xaxis(2.f, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, 2.f);
	Vector3 origin = Vector3::Zero;

	size_t divisions = 20;

	for (size_t i = 0; i <= divisions; ++i)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, Colors::White);
		VertexPositionColor v2(scale + yaxis, Colors::White);
		_lineBatch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, Colors::White);
		VertexPositionColor v2(scale + xaxis, Colors::White);
		_lineBatch->DrawLine(v1, v2);
	}

	_lineBatch->End();
}

void Game::RenderPrimitives(ID3D12GraphicsCommandList *commandList)
{
	for (auto &entry : _primitiveEffects)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = entry.first;
		std::shared_ptr<BasicEffect> primitiveEffect = entry.second;
		std::shared_ptr<GeometricPrimitive> primitiveShape = _primitiveShapes[sceneObject];

		primitiveEffect->SetMatrices(m_world, m_view, m_proj);
		primitiveEffect->Apply(commandList);
		primitiveShape->Draw(commandList);
	}
}

void Game::RenderModels(ID3D12GraphicsCommandList *commandList)
{
}

void Game::RenderMeshes(ID3D12GraphicsCommandList *commandList)
{
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = _deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto rtvDescriptor = _deviceResources->GetRenderTargetView();
    auto dsvDescriptor = _deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto viewport = _deviceResources->GetScreenViewport();
    auto scissorRect = _deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}

#pragma endregion
#pragma region Message Handlers ----------------------------------------------------------------------------------------

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended.
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed.
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
    if (!_deviceResources->WindowSizeChanged(width, height, rotation))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

void Game::ValidateDevice()
{
    _deviceResources->ValidateDevice();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

#pragma endregion
#pragma region Direct3D Resources --------------------------------------------------------------------------------------

std::wstring Game::GetTextureFileName(std::wstring textureName)
{
	std::wstring fileName = L"Resources\\Textures\\";
	fileName = fileName + textureName + L".dds";
	return fileName;
}

std::wstring Game::GetFontFileName(std::wstring fontName)
{
	std::wstring fileName = L"Resources\\Fonts\\";
	fileName = fileName + fontName + L".font";
	return fileName;
}

// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	_device = _deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).  -----------------------------------

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	_graphicsMemory = std::make_unique<GraphicsMemory>(_device);

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
	_descriptorHeap = std::make_unique<DescriptorHeap>(_device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		100);
		//2);


	ResourceUploadBatch resourceUploadBatch(_device);
	resourceUploadBatch.Begin();

	CreateSpriteTextureResources(&resourceUploadBatch);
	CreateTextFontResources(&resourceUploadBatch);
	CreateSceneTextureResources(&resourceUploadBatch);

	CreateSpriteBatch(&resourceUploadBatch);
	CreateTextBatch(&resourceUploadBatch);
	//CreateTriangleBatch();
	//CreateLineBatch();
	CreateSceneResources(&resourceUploadBatch);

	m_world = Matrix::Identity;

	auto uploadResourcesFinished = resourceUploadBatch.End(_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	//	----------------------------------------------------------------------------------------------------------------
}


void Game::CreateSpriteTextureResources(ResourceUploadBatch *resourceUploadBatch)
{
	//	create scene sprites.
	auto sceneSprites = _scene->SceneSprites();
	if (sceneSprites.size() > 0)
	{
		//	Iterate over scene sprites.
		for (auto& sceneSprite : sceneSprites)
		{
			std::shared_ptr<ITurboSceneTexture> texture = sceneSprite->Texture();

			CreateTextureResource(texture, resourceUploadBatch);
		}
	}
}

void Game::CreateSpriteBatch(ResourceUploadBatch *resourceUploadBatch)
{
	//	create scene sprites.
	auto sceneSprites = _scene->SceneSprites();
	if (sceneSprites.size() > 0)
	{
		RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		_spriteStates = std::make_unique<CommonStates>(_device);
		auto samplerDescriptor = _spriteStates->LinearWrap();
		SpriteBatchPipelineStateDescription pd(rtState, nullptr, nullptr, nullptr, &samplerDescriptor);
		_spriteBatch = std::make_unique<SpriteBatch>(_device, *resourceUploadBatch, pd);
	}
}

void Game::CreateTextFontResources(ResourceUploadBatch *resourceUploadBatch)
{
	auto sceneTexts = _scene->SceneTexts();
	if (sceneTexts.size() > 0)
	{
		for (auto& sceneText : sceneTexts)
		{
			std::shared_ptr<ITurboSceneFont> font = sceneText->Font();

			CreateFontResource(font, resourceUploadBatch);
		}
	}
}

void Game::CreateTextBatch(ResourceUploadBatch *resourceUploadBatch)
{
	auto sceneTexts = _scene->SceneTexts();
	if (sceneTexts.size() > 0)
	{
		RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		SpriteBatchPipelineStateDescription pd(rtState);
		_textBatch = std::make_unique<SpriteBatch>(_device, *resourceUploadBatch, pd);
	}
}

void Game::CreateTriangleBatch()
{
	_triangleBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(_device);

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	EffectPipelineStateDescription pd(
		&VertexPositionColor::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState);

	_triangleEffect = std::make_unique<BasicEffect>(_device, EffectFlags::VertexColor, pd);
}

void Game::CreateLineBatch()
{
	m_world = Matrix::Identity;

	_lineBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(_device);

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	EffectPipelineStateDescription pd(
		&VertexPositionColor::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

	_lineEffect = std::make_unique<BasicEffect>(_device, EffectFlags::VertexColor, pd);
}

//	https://github.com/Microsoft/DirectXTK12/wiki/3D-shapes
void Game::CreateSceneTextureResources(ResourceUploadBatch *resourceUploadBatch)
{
	auto sceneObjects = _scene->SceneObjects();

	//	Iterate over scene objects.
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectTextureResource(sceneObject, resourceUploadBatch);
	}
}

void Game::CreateSceneObjectTextureResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch)
{
	std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
	if (material != nullptr)
	{
		std::shared_ptr<ITurboSceneTexture> texture = material->Texture();
		if (texture != nullptr)
		{
			CreateTextureResource(texture, resourceUploadBatch);
		}
	}

	auto sceneObjects = sceneObject->ChildSceneObjects();

	//	Iterate over scene objects.
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectTextureResource(sceneObject, resourceUploadBatch);
	}
}

void Game::CreateSceneResources(ResourceUploadBatch *resourceUploadBatch)
{
	auto sceneObjects = _scene->SceneObjects();

	//	Iterate over scene objects.
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectResource(sceneObject, resourceUploadBatch);
	}
}

void Game::CreateSceneObjectResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch)
{
	switch (sceneObject->PrimitiveType())
	{
	case SceneObjectPrimitiveType::None:
		break;
	case SceneObjectPrimitiveType::Mesh:
		//CreateSceneObjectMeshResource(sceneObject, resourceUploadBatch);
		break;
	case SceneObjectPrimitiveType::Model:
		//CreateSceneObjectModelResource(sceneObject, resourceUploadBatch);
		break;
	default:
		CreateSceneObjectPrimitiveResource(sceneObject, resourceUploadBatch);
	}

	auto sceneObjects = sceneObject->ChildSceneObjects();

	//	Iterate over scene objects.
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectResource(sceneObject, resourceUploadBatch);
	}
}

void Game::CreateSceneObjectMeshResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch)
{
	if (sceneObject->PrimitiveType() != SceneObjectPrimitiveType::Mesh)
	{
		return;
	}
}

void Game::CreateSceneObjectModelResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch)
{
	if (sceneObject->PrimitiveType() != SceneObjectPrimitiveType::Mesh)
	{
		return;
	}
}

void Game::CreateSceneObjectPrimitiveResource(std::shared_ptr<ITurboSceneObject> sceneObject, ResourceUploadBatch *resourceUploadBatch)
{
	switch (sceneObject->PrimitiveType())
	{
	case SceneObjectPrimitiveType::None:
		return;
	case SceneObjectPrimitiveType::Mesh:
		return;
	case SceneObjectPrimitiveType::Model:
		return;
	default:
		break;
	}

	std::shared_ptr<ITurboSceneTexture> texture = nullptr;
	size_t textureDescriptor = 0;
	std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
	if (material != nullptr)
	{
		texture = material->Texture();
		if (texture != nullptr)
		{
			std::wstring textureName = ToWString(texture->Name());
			textureDescriptor = _sceneTextureDescriptors[textureName];
		}
	}

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	_primitiveStates = std::make_unique<CommonStates>(_device);

	EffectPipelineStateDescription psd(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState);

	std::shared_ptr<BasicEffect> primitiveEffect = std::shared_ptr<BasicEffect>(
		new BasicEffect(_device, EffectFlags::PerPixelLighting | EffectFlags::Texture, psd));
	_primitiveEffects[sceneObject] = primitiveEffect;
	primitiveEffect->SetLightEnabled(0, true);
	primitiveEffect->SetLightDiffuseColor(0, Colors::White);
	primitiveEffect->SetLightDirection(0, -Vector3::UnitZ);

	if (texture != nullptr)
	{
		primitiveEffect->SetTexture(
			_descriptorHeap->GetGpuHandle(textureDescriptor), 
			_primitiveStates->AnisotropicWrap());
	}

	switch (sceneObject->PrimitiveType())
	{
	case SceneObjectPrimitiveType::Sphere:
		_primitiveShapes[sceneObject] = GeometricPrimitive::CreateSphere();
		break;
	case SceneObjectPrimitiveType::Cube:
		_primitiveShapes[sceneObject] = GeometricPrimitive::CreateCube();
		break;
	default:
		break;
	}
}

void Game::CreateTextureResource(std::shared_ptr<ITurboSceneTexture> texture, ResourceUploadBatch *resourceUploadBatch)
{
	if (texture == nullptr)
	{
		return;
	}

	std::wstring textureName = ToWString(texture->Name());

	//  Already loaded this texture? don't reload it.
	if (_sceneTextureDescriptors.find(textureName) != _sceneTextureDescriptors.end())
	{
		return;
	}

	size_t textureDescriptor = _descriptorCount++;
	_sceneTextureDescriptors[textureName] = textureDescriptor;

	std::wstring textureFileName = GetTextureFileName(textureName);

	ComPtr<ID3D12Resource> textureResource;

	ThrowIfFailed(
		CreateDDSTextureFromFile(_device, *resourceUploadBatch, textureFileName.c_str(),
			textureResource.ReleaseAndGetAddressOf()));

	CreateShaderResourceView(_device, textureResource.Get(),
		_descriptorHeap->GetCpuHandle(textureDescriptor));

	_sceneTextureResources[textureName] = textureResource;
}

void Game::CreateFontResource(std::shared_ptr<ITurboSceneFont> font, ResourceUploadBatch *resourceUploadBatch)
{
	if (font == nullptr)
	{
		return;
	}

	std::wstring fontName = ToWString(font->Name());

	//  Already loaded this texture? don't reload it.
	if (_sceneFontDescriptors.find(fontName) != _sceneFontDescriptors.end())
	{
		return;
	}

	size_t fontDescriptor = _descriptorCount++;
	_sceneFontDescriptors[fontName] = fontDescriptor;

	std::wstring fontFileName = GetFontFileName(fontName);

	std::shared_ptr<SpriteFont> fontResource = std::shared_ptr<SpriteFont>(new SpriteFont(
		_device,
		*resourceUploadBatch,
		fontFileName.c_str(),
		_descriptorHeap->GetCpuHandle(fontDescriptor),
		_descriptorHeap->GetGpuHandle(fontDescriptor)));

	_sceneFontResources[fontName] = fontResource;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here. -----------------------------------------------------------

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
	auto size = _deviceResources->GetOutputSize();
	D3D12_VIEWPORT viewport =
	{ 
		0.0f, 
		0.0f,
		static_cast<float>(size.right), 
		static_cast<float>(size.bottom),
		D3D12_MIN_DEPTH, 
		D3D12_MAX_DEPTH 
	};

	_spriteBatch->SetViewport(viewport);
	_textBatch->SetViewport(viewport);

	_fullscreenRect.left = 0;
	_fullscreenRect.top = 0;
	_fullscreenRect.right = size.right;
	_fullscreenRect.bottom = size.bottom;


	m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(size.right) / float(size.bottom), 0.1f, 10.f);

	//_lineEffect->SetView(m_view);
	//_lineEffect->SetProjection(m_proj);

	//	----------------------------------------------------------------------------------------------------------------
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	_graphicsMemory.reset();

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures

	_descriptorHeap.reset();

	_spriteStates.reset();
	_spriteBatch.reset();
	_textBatch.reset();

	_triangleEffect.reset();
	_triangleBatch.reset();

	_primitiveStates.reset();

	//	TODO: iterate through _sceneTextureResources and reset and free up the texture resources.
	//	TODO: iterate through _sceneFontResources and reset and free up the font resources.
	//	TODO: iterate through _primitiveEffects and reset and free up the primitive resources.
	//	TODO: iterate through _primitiveShapes and reset and free up the primitive resources.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
