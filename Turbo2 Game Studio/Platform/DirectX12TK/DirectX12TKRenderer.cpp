
#include <pch.h>

#include <ITurboImage.h>
#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>

#include <DirectX12TKRenderer.h>
#include <Windows10Helpers.h>

using namespace Concurrency;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::DirectX12TK;
using namespace Turbo::Platform::Windows10;

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
DirectX12TKRenderer::DirectX12TKRenderer(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService) :
	_debug(debug),
	_ioService(ioService)
	//_constantBufferMappedResource(nullptr)
{
}

DirectX12TKRenderer::~DirectX12TKRenderer()
{
	ReleaseSceneResources();
}

#pragma endregion
#pragma region Methods -------------------------------------------------------------------------------------------------

void DirectX12TKRenderer::UpdateDisplayInformation()
{
	_deviceResources->UpdateDisplayInformation();
}

bool DirectX12TKRenderer::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
	ReleaseSceneResources();

	if (_deviceResources == nullptr)
	{
		_deviceResources = std::make_unique<DeviceResources>();
		_deviceResources->RegisterDeviceNotify(this);
		_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
		_device = _deviceResources->GetD3DDevice();

		//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
		_graphicsMemory = std::make_unique<GraphicsMemory>(_device);
	}

	GetSceneResourceNames(scene);
	CreateDescriptorHeap();

	_resourceUploadBatch = new ResourceUploadBatch(_device);
	_resourceUploadBatch->Begin();

	CreateTextureResources();
	CreateFontResources();

	CreateSpriteBatch(scene);
	CreateTextBatch(scene);
	CreateSceneResources(scene);

	UpdateProjectionMatrix();

	m_world = Matrix::Identity;

	auto uploadResourcesFinished = _resourceUploadBatch->End(_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool DirectX12TKRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
	// Don't try to render anything before the first Update.
	//if (m_timer.GetFrameCount() == 0)
	//{
	//	return;
	//}

	// Prepare the command list to render a new frame.
	_deviceResources->Prepare();
	Clear();

	_commandList = _deviceResources->GetCommandList();
	PIXBeginEvent(_commandList, PIX_COLOR_DEFAULT, L"Render");
	{
		UpdateViewMatrix(scene->CameraPlacement(), scene->LightHack());

		InitializeRender();

		RenderMeshes(scene);
		RenderSprites(scene);
		RenderText(scene);
		RenderPrimitives(scene);
		RenderModels(scene);

	}
	PIXEndEvent(_commandList);

	// Show the new frame.
	PIXBeginEvent(_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
	{
		FinalizeRender();
	}
	PIXEndEvent(_deviceResources->GetCommandQueue());

	return true;
}

#pragma endregion
#pragma region IDeviceNotify Methods -----------------------------------------------------------------------------------

void DirectX12TKRenderer::OnDeviceLost()
{
	ReleaseSceneResources();
}

void DirectX12TKRenderer::OnDeviceRestored()
{
	//CreateDeviceDependentResources();

	//CreateWindowSizeDependentResources();

	UpdateProjectionMatrix();
}

#pragma endregion
#pragma region Scene Resource Support Methods --------------------------------------------------------------------------

void DirectX12TKRenderer::ReleaseSceneResources()
{
	if (_deviceResources != nullptr) 
	{
		_deviceResources->WaitForGpu();
		_deviceResources = nullptr;
	}

	// TODO: Add Direct3D resource cleanup here.

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	_graphicsMemory.reset();

	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
	_descriptorHeap.reset();
	_commonStates.reset();

	_spriteBatch.reset();
	_textBatch.reset();

	//	TODO: iterate through _sceneTextureResources and reset and free up the texture resources.
	//	TODO: iterate through _sceneFontResources and reset and free up the font resources.
	//	TODO: iterate through _primitiveEffects and reset and free up the primitive resources.
	//	TODO: iterate through _primitiveShapes and reset and free up the primitive resources.
}

//	Scene Texture Resources --------------------------------------------------------------------------------------------

void DirectX12TKRenderer::GetSceneResourceNames(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	//	Prepare data structures.
	_sceneTextureNames.clear();
	_sceneTextureResources.clear();
	_sceneTextureDescriptors.clear();
	_sceneFontNames.clear();
	_sceneFontResources.clear();
	_sceneFontDescriptors.clear();
	
	//	Scene Sprite Textures
	auto sceneSprites = scene->SceneSprites();
	for (auto& sceneSprite : sceneSprites)
	{
		GetSceneSpriteTextureName(sceneSprite);
	}

	//	Scene Object Textures
	auto sceneObjects = scene->SceneObjects();
	for (auto& sceneObject : sceneObjects)
	{
		GetSceneObjectTextureNames(sceneObject);
	}

	//	Scene Text Fonts
	auto sceneTexts = scene->SceneTexts();

	for (auto& sceneText : sceneTexts)
	{
		GetSceneTextFontName(sceneText);
	}
}

void DirectX12TKRenderer::GetSceneSpriteTextureName(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
	std::shared_ptr<ITurboSceneTexture> texture = sceneSprite->Texture();
	if (texture == nullptr)
	{
		return;
	}

	std::wstring textureName = ToWString(texture->Name());
	if (textureName.empty())
	{
		return;
	}

	//  Already loaded this texture? don't reload it.
	if (std::find(_sceneTextureNames.begin(), _sceneTextureNames.end(), textureName)
		!= _sceneTextureNames.end())
	{
		return;
	}

	_sceneTextureNames.push_back(textureName);
}

void DirectX12TKRenderer::GetSceneObjectTextureNames(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	GetSceneObjectTextureName(sceneObject);

	//  Iterate over child scene objects. Call this method recursively.
	auto childSceneObjects = sceneObject->ChildSceneObjects();
	for (auto& childSceneObject : childSceneObjects)
	{
		GetSceneObjectTextureNames(childSceneObject);
	}
}

void DirectX12TKRenderer::GetSceneObjectTextureName(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneTexture> texture = material->Texture();
	if (texture == nullptr)
	{
		return;
	}

	std::wstring textureName = ToWString(texture->Name());
	if (textureName.empty())
	{
		return;
	}

	//  Already loaded this texture? don't reload it.
	if (std::find(_sceneTextureNames.begin(), _sceneTextureNames.end(), textureName)
		!= _sceneTextureNames.end())
	{
		return;
	}

	_sceneTextureNames.push_back(textureName);
}

void DirectX12TKRenderer::GetSceneTextFontName(std::shared_ptr<ITurboSceneText> sceneText)
{
	std::shared_ptr<ITurboSceneFont> font = sceneText->Font();
	if (font == nullptr)
	{
		return;
	}

	std::wstring fontName = ToWString(font->Name());
	if (fontName.empty())
	{
		return;
	}

	//  Already loaded this font? don't reload it.
	if (std::find(_sceneFontNames.begin(), _sceneFontNames.end(), fontName)
		!= _sceneFontNames.end())
	{
		return;
	}

	_sceneFontNames.push_back(fontName);
}

void DirectX12TKRenderer::CreateDescriptorHeap()
{
	//	https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures
	_descriptorHeap = std::make_unique<DescriptorHeap>(_device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		_sceneTextureNames.size() + 
		_sceneFontNames.size());
}

void DirectX12TKRenderer::CreateTextureResources()
{
	for (auto& textureName : _sceneTextureNames)
	{
		size_t textureDescriptor = _descriptorCount++;
		_sceneTextureDescriptors[textureName] = textureDescriptor;

		std::wstring textureFileName = GetTextureFileName(textureName);

		ComPtr<ID3D12Resource> textureResource;

		ThrowIfFailed(
			CreateDDSTextureFromFile(
				_device, 
				*_resourceUploadBatch, 
				textureFileName.c_str(),
				textureResource.ReleaseAndGetAddressOf()));

		CreateShaderResourceView(
			_device, 
			textureResource.Get(),
			_descriptorHeap->GetCpuHandle(textureDescriptor));

		_sceneTextureResources[textureName] = textureResource;
	}
}

void DirectX12TKRenderer::CreateFontResources()
{
	for (auto& fontName : _sceneFontNames)
	{
		size_t fontDescriptor = _descriptorCount++;
		_sceneFontDescriptors[fontName] = fontDescriptor;

		std::wstring fontFileName = GetFontFileName(fontName);

		std::shared_ptr<SpriteFont> fontResource = std::shared_ptr<SpriteFont>(new SpriteFont(
			_device,
			*_resourceUploadBatch,
			fontFileName.c_str(),
			_descriptorHeap->GetCpuHandle(fontDescriptor),
			_descriptorHeap->GetGpuHandle(fontDescriptor)));

		_sceneFontResources[fontName] = fontResource;
	}
}

void DirectX12TKRenderer::CreateSpriteBatch(std::shared_ptr<ITurboScene> scene)
{
	//	create scene sprites.
	auto sceneSprites = scene->SceneSprites();
	if (sceneSprites.size() > 0)
	{
		if (_commonStates == nullptr)
		{
			_commonStates = std::make_unique<CommonStates>(_device);
		}

		RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		auto samplerDescriptor = _commonStates->LinearWrap();
		SpriteBatchPipelineStateDescription pd(rtState, nullptr, nullptr, nullptr, &samplerDescriptor);
		_spriteBatch = std::make_unique<SpriteBatch>(_device, *_resourceUploadBatch, pd);
	}
}

void DirectX12TKRenderer::CreateTextBatch(std::shared_ptr<ITurboScene> scene)
{
	auto sceneTexts = scene->SceneTexts();
	if (sceneTexts.size() > 0)
	{
		RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		SpriteBatchPipelineStateDescription pd(rtState);
		_textBatch = std::make_unique<SpriteBatch>(_device, *_resourceUploadBatch, pd);
	}
}

void DirectX12TKRenderer::CreateSceneResources(std::shared_ptr<ITurboScene> scene)
{
	//	Iterate over scene objects.
	auto sceneObjects = scene->SceneObjects();
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectResource(sceneObject);
	}
}

void DirectX12TKRenderer::CreateSceneObjectResource(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	switch (sceneObject->PrimitiveType())
	{
	case SceneObjectPrimitiveType::None:
		break;
	case SceneObjectPrimitiveType::Mesh:
		CreateSceneObjectMeshResource(sceneObject);
		break;
	case SceneObjectPrimitiveType::Model:
		CreateSceneObjectModelResource(sceneObject);
		break;
	default:
		CreateSceneObjectPrimitiveResource(sceneObject);
		break;
	}

	//  Iterate over child scene objects. Call this method recursively.
	auto sceneObjects = sceneObject->ChildSceneObjects();
	for (auto &sceneObject : sceneObjects)
	{
		CreateSceneObjectResource(sceneObject);
	}
}

void DirectX12TKRenderer::CreateSceneObjectMeshResource(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	if (sceneObject->PrimitiveType() != SceneObjectPrimitiveType::Mesh)
	{
		return;
	}

	std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

	if (mesh == nullptr)
	{
		return;
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
	if (_commonStates == nullptr)
	{
		_commonStates = std::make_unique<CommonStates>(_device);
	}

	EffectPipelineStateDescription psd(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState);

	std::shared_ptr<BasicEffect> meshEffect = std::shared_ptr<BasicEffect>(
		new BasicEffect(_device, EffectFlags::PerPixelLighting | EffectFlags::Texture, psd));
	_primitiveEffects[sceneObject] = meshEffect;
	meshEffect->SetLightEnabled(0, true);
	meshEffect->SetLightDiffuseColor(0, Colors::White);
	meshEffect->SetLightDirection(0, -Vector3::UnitZ);

	if (texture != nullptr)
	{
		meshEffect->SetTexture(
			_descriptorHeap->GetGpuHandle(textureDescriptor),
			_commonStates->AnisotropicWrap());
	}

	//_primitiveShapes[sceneObject] = GeometricPrimitive::CreateSphere();
	//return;

	//	mesh is already created?
	if (_meshPrimitiveShapes.find(mesh) != _meshPrimitiveShapes.end())
	{
		return;
	}

	std::vector<VertexPositionNormalTexture> vertexList;
	std::vector<uint16_t> indexList;

	for (auto& meshVertex : mesh->Vertices())
	{
		VertexPositionNormalTexture shaderVertex;

		shaderVertex.position = XMFLOAT3((float)(meshVertex.Position.X), (float)(meshVertex.Position.Y), (float)(meshVertex.Position.Z));
		shaderVertex.normal = XMFLOAT3((float)(meshVertex.Normal.X), (float)(meshVertex.Normal.Y), (float)(meshVertex.Normal.Z));
		shaderVertex.textureCoordinate = XMFLOAT2((float)(meshVertex.TextureUV.X), (float)(-meshVertex.TextureUV.Y));

		vertexList.push_back(shaderVertex);
	}

	for (auto& meshTriangle : mesh->Triangles())
	{
		indexList.push_back(meshTriangle.Vertex1);
		indexList.push_back(meshTriangle.Vertex2);
		indexList.push_back(meshTriangle.Vertex3);
	}

	//_primitiveShapes[sceneObject] = GeometricPrimitive::CreateCustom(vertexList, indexList);
	_meshPrimitiveShapes[mesh] = GeometricPrimitive::CreateCustom(vertexList, indexList);
}

void DirectX12TKRenderer::CreateSceneObjectModelResource(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	if (sceneObject->PrimitiveType() != SceneObjectPrimitiveType::Model)
	{
		return;
	}
}

void DirectX12TKRenderer::CreateSceneObjectPrimitiveResource(std::shared_ptr<ITurboSceneObject> sceneObject)
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
	if (_commonStates == nullptr)
	{
		_commonStates = std::make_unique<CommonStates>(_device);
	}

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
			_commonStates->AnisotropicWrap());
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

std::wstring DirectX12TKRenderer::GetTextureFileName(std::wstring textureName)
{
	std::wstring fileName = L"Resources\\Textures\\";
	fileName = fileName + textureName + L".dds";
	return fileName;
}

std::wstring DirectX12TKRenderer::GetFontFileName(std::wstring fontName)
{
	std::wstring fileName = L"Resources\\Fonts\\";
	fileName = fileName + fontName + L".font";
	return fileName;
}


#pragma endregion
#pragma region Rendering Support Methods -------------------------------------------------------------------------------

// Helper method to clear the back buffers.
void DirectX12TKRenderer::Clear()
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

void DirectX12TKRenderer::UpdateProjectionMatrix()
{
	// Initializes view parameters when the window size changes.

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

	if (_spriteBatch != nullptr)
	{
		_spriteBatch->SetViewport(viewport);
	}
	
	if (_textBatch != nullptr) 
	{
		_textBatch->SetViewport(viewport);
	}

	_fullscreenRect.left = 0;
	_fullscreenRect.top = 0;
	_fullscreenRect.right = size.right;
	_fullscreenRect.bottom = size.bottom;

	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(size.right) / float(size.bottom), 0.1f, 100.f);
}

void DirectX12TKRenderer::UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack)
{
	TurboVector3D position = cameraPlacement->Position();
	TurboVector3D target = cameraPlacement->Target();
	TurboVector3D up = cameraPlacement->Up();

	Vector3 DX_eye((float)(position.X), (float)(position.Y), (float)(position.Z));
	Vector3 DX_target((float)(target.X), (float)(target.Y), (float)(target.Z));
	Vector3 DX_up((float)(up.X), (float)(up.Y), (float)(up.Z));

	m_view = Matrix::CreateLookAt(DX_eye, DX_target, DX_up);
}

void DirectX12TKRenderer::InitializeRender()
{
	std::vector<ID3D12DescriptorHeap*> heaps;

	heaps.push_back(_descriptorHeap->Heap());

	if (_commonStates != nullptr)
	{
		heaps.push_back(_commonStates->Heap());
	}

	_commandList->SetDescriptorHeaps(heaps.size(), &heaps[0]);
}

void DirectX12TKRenderer::RenderSprites(std::shared_ptr<ITurboScene> scene)
{
	auto sceneSprites = scene->SceneSprites();

	if (sceneSprites.size() > 0)
	{
		_spriteBatch->Begin(_commandList);

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

void DirectX12TKRenderer::RenderText(std::shared_ptr<ITurboScene> scene)
{
	auto sceneTexts = scene->SceneTexts();

	if (sceneTexts.size() > 0)
	{
		_textBatch->Begin(_commandList);

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

void DirectX12TKRenderer::RenderPrimitives(std::shared_ptr<ITurboScene> scene)
{
	for (auto &entry : _primitiveEffects)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = entry.first;
		std::shared_ptr<BasicEffect> primitiveEffect = entry.second;
		std::shared_ptr<GeometricPrimitive> primitiveShape = _primitiveShapes[sceneObject];

		if (primitiveShape == nullptr)
		{
			continue;
		}

		TurboMatrix4x4 transform = sceneObject->Placement()->Transform();

		Matrix model(
			(float)transform.M11, (float)transform.M12, (float)transform.M13, (float)transform.M14,
			(float)transform.M21, (float)transform.M22, (float)transform.M23, (float)transform.M24,
			(float)transform.M31, (float)transform.M32, (float)transform.M33, (float)transform.M34,
			(float)transform.M41, (float)transform.M42, (float)transform.M43, (float)transform.M44);
		Matrix global = Matrix::Identity;

		TurboVector3D direction = (sceneObject->Placement()->Position()
								-  scene->CameraPlacement()->Position()).Normalize();
		Vector3 lightDirection = Vector3(direction.X, direction.Y, direction.Z);

		primitiveEffect->SetLightDirection(0, lightDirection);

		primitiveEffect->SetMatrices(model, m_view, m_proj);
		primitiveEffect->Apply(_commandList);
		primitiveShape->Draw(_commandList);
	}
}

void DirectX12TKRenderer::RenderModels(std::shared_ptr<ITurboScene> scene)
{
}

void DirectX12TKRenderer::RenderMeshes(std::shared_ptr<ITurboScene> scene)
{
	auto sceneObjects = scene->SceneObjects();
	for (auto &sceneObject : sceneObjects)
	{
		RenderSceneObjectMeshes(scene, sceneObject);
	}
}

void DirectX12TKRenderer::RenderSceneObjectMeshes(std::shared_ptr<ITurboScene> scene, std::shared_ptr<ITurboSceneObject> sceneObject)
{
	RenderSceneObjectMesh(scene, sceneObject);

	auto childSceneObjects = sceneObject->ChildSceneObjects();
	for (auto & childSceneObject : childSceneObjects)
	{
		RenderSceneObjectMeshes(scene, childSceneObject);
	}
}

void DirectX12TKRenderer::RenderSceneObjectMesh(std::shared_ptr<ITurboScene> scene, std::shared_ptr<ITurboSceneObject> sceneObject)
{
	if (sceneObject->PrimitiveType() != SceneObjectPrimitiveType::Mesh)
	{
		return;
	}

	auto mesh = sceneObject->Mesh();
	if (mesh == nullptr)
	{
		return;
	}

	std::shared_ptr<GeometricPrimitive> primitiveShape = _meshPrimitiveShapes[mesh];
	if (primitiveShape == nullptr)
	{
		return;
	}
	
	std::shared_ptr<BasicEffect> primitiveEffect = _primitiveEffects[sceneObject];
	if (primitiveEffect == nullptr)
	{
		return;
	}

	TurboMatrix4x4 transform = sceneObject->Placement()->Transform();

	TurboVector3D direction = sceneObject->Placement()->Position() - scene->CameraPlacement()->Position();
	direction = direction.Normalize();
	Vector3 lightDirection = Vector3(direction.X, direction.Y, direction.Z);

	Matrix model(
		(float)transform.M11, (float)transform.M12, (float)transform.M13, (float)transform.M14,
		(float)transform.M21, (float)transform.M22, (float)transform.M23, (float)transform.M24,
		(float)transform.M31, (float)transform.M32, (float)transform.M33, (float)transform.M34,
		(float)transform.M41, (float)transform.M42, (float)transform.M43, (float)transform.M44);

	primitiveEffect->SetLightDirection(0, lightDirection);
	primitiveEffect->SetMatrices(model, m_view, m_proj);
	primitiveEffect->Apply(_commandList);
	primitiveShape->Draw(_commandList);
}

void DirectX12TKRenderer::FinalizeRender()
{
	_deviceResources->Present();

	//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
	_graphicsMemory->Commit(_deviceResources->GetCommandQueue());
}

#pragma endregion

