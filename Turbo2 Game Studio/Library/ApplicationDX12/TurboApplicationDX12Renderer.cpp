
#include "pch.h"
#include "TurboApplicationDX12Renderer.h"

#include "..\Common\DirectXHelper.h"
#include <ppltasks.h>
#include <synchapi.h>

using namespace Application_DX12;

using namespace Concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Core;

#pragma region Constructors and Destructors  ---------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
TurboApplicationDX12Renderer::TurboApplicationDX12Renderer() :
	_constantBufferMappedResource(nullptr)
{
	_deviceResources = std::make_shared<DX::DeviceResources>();
	_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
	_device = _deviceResources->GetD3DDevice();
}

TurboApplicationDX12Renderer::~TurboApplicationDX12Renderer()
{
	_constantBufferTargetResource->Unmap(0, nullptr);
	_constantBufferMappedResource = nullptr;
}

#pragma endregion
#pragma region ITurboApplicationDX12Renderer Methods  ------------------------------------------------------------------

void Application_DX12::TurboApplicationDX12Renderer::Resize(float width, float height)
{
	_deviceResources->SetLogicalSize(Size(width, height));
}

void Application_DX12::TurboApplicationDX12Renderer::SetDPI(float logicalDPI)
{
	_deviceResources->SetDpi(logicalDPI);
}

void Application_DX12::TurboApplicationDX12Renderer::SetDisplayOrientation(Windows::Graphics::Display::DisplayOrientations displayOrientation)
{
	_deviceResources->SetCurrentOrientation(displayOrientation);
}

void Application_DX12::TurboApplicationDX12Renderer::ValidateDevice()
{
	_deviceResources->ValidateDevice();
}

bool Application_DX12::TurboApplicationDX12Renderer::NeedsReset()
{
	return _deviceResources->IsDeviceRemoved();
}

bool TurboApplicationDX12Renderer::LoadLevelResources(std::shared_ptr<IGameLevel> level)
{
	CreateRootSignature();
	CreatePipelineStateObject();
	CreateCommandList();

	CreateLevelVertexResources(level);
	CreateLevelTextureResources(level);
	CreateConstantBufferResources();

	CreateCBVSRVDescriptorHeap();
	CreateCBVDescriptors();
	CreateSRVDescriptors();

	CreateSamplerDescriptorHeap();
	CreateSamplerDescriptor();

	ExecuteCommandList();

	//	Wait for the command list to finish executing; 
	//	the vertex/index buffers need to be uploaded to the GPU before the upload resources go out of scope.
	_deviceResources->WaitForGpu();

	//auto commandQueue = _deviceResources->GetCommandQueue();
	//PIXBeginEvent(commandQueue, 0, L"Update");
	//{
	//}
	//PIXEndEvent(commandQueue);

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool TurboApplicationDX12Renderer::RenderLevel(std::shared_ptr<IGameLevel> level)
{
	auto commandQueue = _deviceResources->GetCommandQueue();
	PIXBeginEvent(commandQueue, 0, L"Render");
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix(level->StaticScene()->CameraPlacement());

		InitializeRendering();

		PopulateCommandList(level);

		FinalizeRendering();
	}
	PIXEndEvent(commandQueue);

	return true;
}

#pragma endregion
#pragma region Local Support Methods  ----------------------------------------------------------------------------------

void Application_DX12::TurboApplicationDX12Renderer::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE descriptorRanges[3];
	//descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
	//descriptorRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	////descriptorRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	//descriptorRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descriptorRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	descriptorRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParameters[3];
	//rootParameters[0].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	//rootParameters[1].InitAsDescriptorTable(1, &descriptorRanges[1], D3D12_SHADER_VISIBILITY_VERTEX);
	////rootParameters[2].InitAsDescriptorTable(1, &descriptorRanges[2], D3D12_SHADER_VISIBILITY_VERTEX);
	//rootParameters[2].InitAsDescriptorTable(1, &descriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[0].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_VERTEX);
	rootParameters[1].InitAsDescriptorTable(1, &descriptorRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[2].InitAsDescriptorTable(1, &descriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the constant buffer.
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
	descRootSignature.Init(
		_countof(rootParameters), rootParameters, 
		0, nullptr, 
		rootSignatureFlags);

	ComPtr<ID3DBlob> pSignature;
	ComPtr<ID3DBlob> pError;

	DX::ThrowIfFailed(D3D12SerializeRootSignature(
		&descRootSignature, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		pSignature.GetAddressOf(), 
		pError.GetAddressOf()));

	DX::ThrowIfFailed(_device->CreateRootSignature(
		0, 
		pSignature->GetBufferPointer(), 
		pSignature->GetBufferSize(), 
		IID_PPV_ARGS(&_rootSignature)));
}

void TurboApplicationDX12Renderer::CreatePipelineStateObject()
{
	UINT8* pVertexShaderData;
	UINT8* pPixelShaderData;
	UINT vertexShaderDataLength;
	UINT pixelShaderDataLength;

	//  Load the shaders.
	DX::ThrowIfFailed(ReadDataFromFile(
		L"VertexShader.cso", 
		&pVertexShaderData, 
		&vertexShaderDataLength));

	DX::ThrowIfFailed(ReadDataFromFile(
		L"PixelShader.cso", 
		&pPixelShaderData, 
		&pixelShaderDataLength));

	static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.pRootSignature = _rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(pVertexShaderData, vertexShaderDataLength);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pPixelShaderData, pixelShaderDataLength);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	DX::ThrowIfFailed(_device->CreateGraphicsPipelineState(
		&psoDesc, 
		IID_PPV_ARGS(&_pipelineState)));

	// Shader data can be deleted once the pipeline state is created.
	delete pVertexShaderData;
	delete pPixelShaderData;
}

void Application_DX12::TurboApplicationDX12Renderer::CreateCommandList()
{
	DX::ThrowIfFailed(_device->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		_deviceResources->GetCommandAllocator(), 
		_pipelineState.Get(), 
		IID_PPV_ARGS(&_commandList)));
}

void Application_DX12::TurboApplicationDX12Renderer::ResetCommandList()
{
	DX::ThrowIfFailed(_deviceResources->GetCommandAllocator()->Reset());

	// The command list can be reset anytime after ExecuteCommandList() is called.
	DX::ThrowIfFailed(_commandList->Reset(
		_deviceResources->GetCommandAllocator(), 
		_pipelineState.Get()));
}

void Application_DX12::TurboApplicationDX12Renderer::ExecuteCommandList()
{
	// Close the command list and execute it to begin the vertex/index buffer copy into the GPU's default heap.
	DX::ThrowIfFailed(_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };

	auto commandQueue = _deviceResources->GetCommandQueue();
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

//	Level Vertex Resources ----------------------------------------------------------------------------------------------

void TurboApplicationDX12Renderer::CreateLevelVertexResources(std::shared_ptr<IGameLevel> level)
{
	if (level == nullptr)
	{
		return;
	}

	_levelVertexTargetResources.clear();
	_levelVertexSourceResources.clear();
	_levelVertexBufferViews.clear();

	_levelIndexTargetResources.clear();
	_levelIndexSourceResources.clear();
	_levelIndexBufferViews.clear();

	_levelSceneObjectOffsets.clear();

	auto scene = level->StaticScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			LoadSceneObjectVertices(sceneObject);
			//break;
		}
	}

	scene = level->DynamicScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			LoadSceneObjectVertices(sceneObject);
		}
	}

	_levelSceneObjectCount = _levelSceneObjectOffsets.size();
}

void TurboApplicationDX12Renderer::LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	int sceneObjectOffset = _levelSceneObjectOffsets.size();
	_levelSceneObjectOffsets[sceneObject] = sceneObjectOffset;

	std::vector<ShaderVertex> vertexList;
	std::vector<unsigned short> indexList;

	LoadVertexData(sceneObject->Mesh(), &vertexList, &indexList);

	//  Load mesh vertices  --------------------------------------------------------------------------------------------
	UINT vertexBufferSize = vertexList.size() * sizeof(vertexList[0]);

	// Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
	// The upload resource must not be released until after the GPU has finished using it.
	ComPtr<ID3D12Resource> vertexTargetResource;
	ComPtr<ID3D12Resource> vertexSourceResource;

	//  vertexTargetResource represents an allocated space in GPU memory
	//	for holding the vertex data.
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&vertexTargetResource)));

	//	vertexSourceResource represents an allocated space in CPU memory 
	//	for holding the vertex data to be uploaded to the GPU.
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexSourceResource)));

	vertexTargetResource->SetName(L"Vertex Target Resource");
	vertexSourceResource->SetName(L"Vertex Source Resource");

	_levelVertexTargetResources[sceneObject] = vertexTargetResource;
	_levelVertexSourceResources[sceneObject] = vertexSourceResource;

	//	Copy data to the intermediate upload heap, vertexSourceResource, 
	//	and then schedule a copy from the upload heap to the space allocated in GPU memory, 
	//	identified by vertexTargetResource.
	{
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(vertexList.data());
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexData.RowPitch;

		UpdateSubresources(
			_commandList.Get(),
			vertexTargetResource.Get(),
			vertexSourceResource.Get(),
			0, 0, 1,
			&vertexData);

		CD3DX12_RESOURCE_BARRIER vertexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				vertexTargetResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		_commandList->ResourceBarrier(1, &vertexBufferResourceBarrier);

		// Describe the vertex buffer view. This will be needed in the rendering step.
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
		vertexBufferView.BufferLocation = vertexTargetResource->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(ShaderVertex);
		vertexBufferView.SizeInBytes = vertexBufferSize;
		_levelVertexBufferViews[sceneObject] = vertexBufferView;
	}

	//	Load mesh indices  ---------------------------------------------------------------------------------------------
	UINT indexBufferSize = indexList.size() * sizeof(indexList[0]);

	// Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
	// The upload resource must not be released until after the GPU has finished using it.
	ComPtr<ID3D12Resource> indexTargetResource;
	ComPtr<ID3D12Resource> indexSourceResource;

	//  indexTargetResource represents an allocated space in GPU memory
	//	for holding the index data.
	CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&indexTargetResource)));

	//	indexSourceResource represents an allocated space in CPU memory 
	//	for holding the index data to be uploaded to the GPU.
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexSourceResource)));

	indexTargetResource->SetName(L"Index Buffer Target Resource");
	indexSourceResource->SetName(L"Index Buffer Source Resource");

	_levelIndexTargetResources[sceneObject] = indexTargetResource;
	_levelIndexSourceResources[sceneObject] = indexSourceResource;

	//	Copy data to the intermediate upload heap, indexSourceResource, 
	//	and then schedule a copy from the upload heap to the space allocated in GPU memory, 
	//	identified by indexTargetResource.
	{
		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = reinterpret_cast<BYTE*>(indexList.data());
		indexData.RowPitch = indexBufferSize;
		indexData.SlicePitch = indexData.RowPitch;

		UpdateSubresources(
			_commandList.Get(),
			indexTargetResource.Get(),
			indexSourceResource.Get(),
			0, 0, 1,
			&indexData);

		CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				indexTargetResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_INDEX_BUFFER);

		_commandList->ResourceBarrier(1, &indexBufferResourceBarrier);

		// Describe the index buffer view. This will be needed in the rendering step.
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
		indexBufferView.BufferLocation = indexTargetResource->GetGPUVirtualAddress();
		indexBufferView.SizeInBytes = indexBufferSize;
		indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		_levelIndexBufferViews[sceneObject] = indexBufferView;
	}


	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTextures(childSceneObject);
	}
}

void TurboApplicationDX12Renderer::LoadVertexData(
	std::shared_ptr<ITurboSceneObjectMesh> mesh,
	std::vector<ShaderVertex> *vertexList,
	std::vector<unsigned short> *indexList)
{
	for (auto& meshVertex : mesh->Vertices())
	{
		ShaderVertex shaderVertex;

		shaderVertex.pos = XMFLOAT3((float)(meshVertex.Position.X), (float)(meshVertex.Position.Y), (float)(meshVertex.Position.Z));
		shaderVertex.normal = XMFLOAT3((float)(meshVertex.Normal.X), (float)(meshVertex.Normal.Y), (float)(meshVertex.Normal.Z));
		shaderVertex.texture = XMFLOAT2((float)(meshVertex.TextureUV.X), (float)(meshVertex.TextureUV.Y));

		vertexList->push_back(shaderVertex);
	}

	for (auto& meshTriangle : mesh->Triangles())
	{
		indexList->push_back(meshTriangle.Vertex1);
		indexList->push_back(meshTriangle.Vertex2);
		indexList->push_back(meshTriangle.Vertex3);
	}
}

//	Level Texture Resources ---------------------------------------------------------------------------------------------

void TurboApplicationDX12Renderer::CreateLevelTextureResources(std::shared_ptr<IGameLevel> level)
{
	if (level == nullptr)
	{
		return;
	}

	_levelTextureOffsets.clear();
	_levelTextureTargetResources.clear();
	_levelTextureSourceResources.clear();

	auto scene = level->StaticScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			LoadSceneObjectTextures(sceneObject);
			//break;
		}
	}

	scene = level->DynamicScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			LoadSceneObjectTextures(sceneObject);
		}
	}

	_levelTextureCount = _levelTextureOffsets.size();
}

void TurboApplicationDX12Renderer::LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	//  For now, just one level of scene objects.
	std::string textureName = sceneObject->Material()->Texture()->Name();

	//  Already loaded this texture? don't reload it.
	if (_levelTextureOffsets.find(textureName) != _levelTextureOffsets.end())
	{
		return;
	}

	int textureOffset = _levelTextureOffsets.size();
	_levelTextureOffsets[textureName] = textureOffset;

	D3D12_RESOURCE_DESC textureResourceDesc = {};
	std::vector<unsigned char> textureData;

	LoadTextureData(textureName, &textureResourceDesc, &textureData);

	ComPtr<ID3D12Resource> textureTargetResource;
	ComPtr<ID3D12Resource> textureSourceResource;

	//  textureTargetResource represents an allocated space in GPU memory
	//	for holding the texture data.
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureTargetResource)));

	const UINT subresourceCount = textureResourceDesc.DepthOrArraySize * textureResourceDesc.MipLevels;
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(textureTargetResource.Get(), 0, subresourceCount);

	//	textureSourceResource represents an allocated space in CPU memory
	//	for holding the texture data to be uploaded to the GPU.
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureSourceResource)));

	textureTargetResource->SetName(L"Texture Buffer Target Resource");
	textureSourceResource->SetName(L"Texture Buffer Source Resource");

	_levelTextureTargetResources[textureName] = textureTargetResource;
	_levelTextureSourceResources[textureName] = textureSourceResource;

	//	Copy data to the intermediate upload heap, textureSourceResource, 
	//	and then schedule a copy from the upload heap to the space allocated in GPU memory, 
	//	identified by textureTargetResource.
	D3D12_SUBRESOURCE_DATA materialSubresourceData = {};
	materialSubresourceData.pData = &textureData[0];
	materialSubresourceData.RowPitch = static_cast<LONG_PTR>((4U * textureResourceDesc.Width));
	materialSubresourceData.SlicePitch = materialSubresourceData.RowPitch * textureResourceDesc.Height;

	UpdateSubresources(
		_commandList.Get(), 
		textureTargetResource.Get(), 
		textureSourceResource.Get(), 
		0, 0, subresourceCount, 
		&materialSubresourceData);

	_commandList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(
			textureTargetResource.Get(), 
			D3D12_RESOURCE_STATE_COPY_DEST, 
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	//  Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTextures(childSceneObject);
	}
}

void TurboApplicationDX12Renderer::LoadTextureData(
	std::string textureName,
	D3D12_RESOURCE_DESC *textureResourceDesc,
	std::vector<unsigned char> *textureData)
{
	const UINT textureWidth = 64;
	const UINT textureHeight = 64;
	const UINT texturePixelSize = 4;

	*textureResourceDesc = {};

	textureResourceDesc->MipLevels = 1;
	textureResourceDesc->Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureResourceDesc->Width = textureWidth;
	textureResourceDesc->Height = textureHeight;
	textureResourceDesc->Flags = D3D12_RESOURCE_FLAG_NONE;
	textureResourceDesc->DepthOrArraySize = 1;
	textureResourceDesc->SampleDesc.Count = 1;
	textureResourceDesc->SampleDesc.Quality = 0;
	textureResourceDesc->Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	textureData->resize((size_t)(textureWidth * textureHeight * TexturePixelSize));

	if (textureName == "OriginalCorner")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  grey checker pattern
				int on = (((x + 4) / 8) % 2) * (((y + 4) / 8) % 2);
				float r = on * 0.10f + 0.10f;
				float g = r;
				float b = r * 0.25;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalEdge")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  Gray stripe pattern
				int on = ((y + 4) / 8) % 2;
				float r = on * 0.10f + 0.10f;
				float g = r;
				float b = r * 0.25;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalWall")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  Large Bricks.
				int mortarH = (y % 8) == 0 ? 1 : 0;
				int mortarV = ((y / 8) % 2) ?
					(((x + 4) % 16) == 0 ? 1 : 0) :
					(((x +12) % 16) == 0 ? 1 : 0);
				int mortar = mortarH | mortarV;

				int spot = (rand() % 20) == 0 ? 1 : 0;

				float r = mortar ? 0.15f : (0.25f - spot * 0.05f);
				float g = r;
				float b = r;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalLeftWall")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  Red pattern
				int on = ((x + y) % 2);
				float r = on * 0.25f + 0.25f;
				float g = 0.0f;
				float b = 0.0f;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalRightWall")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  Cyan pattern
				int on = ((x + y) % 2);
				float r = 0.0f;
				float g = on * 0.25f + 0.25f;
				float b = on * 0.25f + 0.25f;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalBackWall")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//	Blue pattern
				int on = ((x + y) % 2);
				float r = 0.0f;
				float g = 0.0f;
				float b = on * 0.25f + 0.25f;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalFrontWall")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//	Yellow pattern
				int on = ((x + y) % 2);
				float r = on * 0.25f + 0.25f;
				float g = on * 0.25f + 0.25f;
				float b = 0.0f;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalCeiling")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//	Green pattern
				int mortar1 = ((x + y + 8) % 16) == 0 ? 1 : 0;
				int mortar2 = ((x - y + 8) % 16) == 0 ? 1 : 0;
				int mortar = mortar1 | mortar2;

				int spot = (rand() % 20) == 0 ? 1 : 0;

				float r = mortar ? 0.20f : (0.30f - spot * 0.05f);
				float g = r;
				float b = r;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
	else if (textureName == "OriginalFloor")
	{
		for (int x = 0; x < textureWidth; ++x)
		{
			for (int y = 0; y < textureHeight; ++y)
			{
				// Compute the appropriate index into the buffer based on the x/y coordinates.
				int pixelIndex = (y * TexturePixelSize * textureWidth) + (x * TexturePixelSize);

				//  Large Bricks.
				int mortarH = ((y + 8) % 16) == 0 ? 1 : 0;
				int mortarV = ((x + 8) % 16) == 0 ? 1 : 0;
				int mortar = mortarH | mortarV;

				int spot = (rand() % 20) == 0 ? 1 : 0;

				float r = mortar ? 0.20f : (0.30f - spot * 0.05f);
				float g = r;
				float b = r;

				(*textureData)[pixelIndex + 0] = static_cast<unsigned char>(255 * r);
				(*textureData)[pixelIndex + 1] = static_cast<unsigned char>(255 * g);
				(*textureData)[pixelIndex + 2] = static_cast<unsigned char>(255 * b);
				(*textureData)[pixelIndex + 3] = 255;
			}
		}
	}
}

//	Level Assets -------------------------------------------------------------------------------------------------------

void TurboApplicationDX12Renderer::CreateConstantBufferResources()
{

	//	This creates a resource in the GPU.
	//	The resource is a Constant Buffer Heap.
	//	This particular Constant Buffer heap is used to store transform matrices 
	//		for the object being drawn.
	//	Enough memory space will be allocated on the GPU to hold as many transform 
	//		matrices as the application requires. For this application, that is three
	//		each for model, view and projection. Three because each frame gets its own 
	//		set of model, view and projection matrices.
	//	The CBV Descriptor Table (parameter 2) in the Root Signature will point 
	//		to a descriptor heap with descriptors that point to this heap resource.
	CD3DX12_RESOURCE_DESC resourceDesc = 
		CD3DX12_RESOURCE_DESC::Buffer(
			(1 + DX::c_frameCount * _levelSceneObjectCount) * c_alignedConstantBufferSize);
	
	DX::ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_constantBufferTargetResource)));
	
	_constantBufferTargetResource->SetName(L"Constant Buffer Target Resource");
}

void TurboApplicationDX12Renderer::CreateCBVSRVDescriptorHeap()
{
	//	Create a descriptor heap for the constant buffer resources (transform matrices) 
	//		and shader resources (textures).

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1 + DX::c_frameCount * _levelSceneObjectCount + _levelTextureCount;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	This flag indicates that this descriptor heap can be bound to the 
	//		pipeline and that descriptors contained in it can be referenced 
	//		by a root table.
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	DX::ThrowIfFailed(_device->CreateDescriptorHeap(
		&descriptorHeapDesc, 
		IID_PPV_ARGS(&_cbvSrvDescriptorHeap)));

	_cbvSrvDescriptorHeap->SetName(L"Constant Buffer View Descriptor Heap");

	_cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TurboApplicationDX12Renderer::CreateCBVDescriptors()
{
	//	Create constant buffer view descriptors to access the upload buffer.
	//	The CBV descriptors point to locations in the CB heap resource created above.
	//	The CBV descriptors are added to the CBV/SRV descriptor heap.
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = _constantBufferTargetResource->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(
		_cbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int n = 0; n < 1 + DX::c_frameCount * _levelSceneObjectCount; n++)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbvGpuAddress;
		cbvDesc.SizeInBytes = c_alignedConstantBufferSize;
		_device->CreateConstantBufferView(&cbvDesc, cbvCpuHandle);

		cbvGpuAddress += cbvDesc.SizeInBytes;
		cbvCpuHandle.Offset(_cbvSrvDescriptorSize);
	}

	//	Map the constant buffers.
	//	The Map() method allocates CPU space that corresponds to the GPU space 
	//		allocated for the Constant Buffer heap resource, and returns a 
	//		pointer to the CPU space allocated.
	CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
	DX::ThrowIfFailed(_constantBufferTargetResource->Map(
		0, 
		&readRange,
		reinterpret_cast<void**>(&_constantBufferMappedResource)));
	
	ZeroMemory(_constantBufferMappedResource, (1 + DX::c_frameCount * _levelSceneObjectCount) * c_alignedConstantBufferSize);
	// We don't unmap this until the app closes. Keeping things mapped for the lifetime of the resource is okay.
}

void TurboApplicationDX12Renderer::CreateSRVDescriptors()
{
	for (auto &levelTextureOffset : _levelTextureOffsets)
	{
		std::string textureName = levelTextureOffset.first;
		int textureOffset = levelTextureOffset.second;
		auto levelTextureTargetResource = _levelTextureTargetResources[textureName];

		CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(
			_cbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1 + DX::c_frameCount * _levelSceneObjectCount + textureOffset,
			_cbvSrvDescriptorSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		_device->CreateShaderResourceView(
			levelTextureTargetResource.Get(), 
			&srvDesc, 
			srvCpuHandle);
	}
}

void TurboApplicationDX12Renderer::CreateSamplerDescriptorHeap()
{
	// Describe and create a sampler descriptor heap.
	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
	samplerHeapDesc.NumDescriptors = 1;
	samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	DX::ThrowIfFailed(_device->CreateDescriptorHeap(
		&samplerHeapDesc, 
		IID_PPV_ARGS(&_samplerDescriptorHeap)));
}

void TurboApplicationDX12Renderer::CreateSamplerDescriptor()
{
	//	Describe and create a sampler.
	//	CreateSampler() creates the sampler resource described by samplerDesc 
	//		and creates a sampler descripter in the first (and only) slot in the sampler descriptor heap,
	//		pointing to the sampler resource.
	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	_device->CreateSampler(
		&samplerDesc, 
		_samplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

//	Rendering Support Methods ------------------------------------------------------------------------------------------

void TurboApplicationDX12Renderer::UpdateProjectionMatrix()
{
	// Initializes view parameters when the window size changes.

	Size outputSize = _deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = _deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = _deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&_constantBufferMappedResource[0].projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	// Prepare to pass the updated model matrix to the shader.
	XMStoreFloat4x4(&_constantBufferMappedResource[0].model, XMMatrixIdentity());
}

void TurboApplicationDX12Renderer::UpdateViewMatrix(std::shared_ptr<ITurboSceneObjectPlacement> cameraPlacement)
{
	Vector3D position = cameraPlacement->Position();
	Vector3D target = cameraPlacement->Target();
	Vector3D up = cameraPlacement->Up();

	XMVECTORF32 eyePosition = { (float)(position.X), (float)(position.Y), (float)(position.Z), 0.0f };
	XMVECTORF32 focusPosition = { (float)(target.X), (float)(target.Y), (float)(target.Z), 0.0f };
	XMVECTORF32 upDirection = { (float)(up.X), (float)(up.Y), (float)(up.Z), 0.0f };

	XMStoreFloat4x4(&_constantBufferMappedResource[0].view,
		XMMatrixTranspose(XMMatrixLookAtRH(eyePosition, focusPosition, upDirection)));

	// Update the constant buffer resource.
	//UINT8* destination = _constantBufferMappedResource
	//	+ (_deviceResources->GetCurrentFrameIndex() * c_alignedConstantBufferSize);

	//memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));
}

void TurboApplicationDX12Renderer::InitializeRendering()
{
	ResetCommandList();

	PIXBeginEvent(_commandList.Get(), 0, L"Draw the scene");

	// Set the graphics root signature and descriptor heaps to be used by this frame.
	_commandList->SetGraphicsRootSignature(_rootSignature.Get());

	ID3D12DescriptorHeap* ppHeaps[] =
	{
		_cbvSrvDescriptorHeap.Get(),
		_samplerDescriptorHeap.Get()
	};
	_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// Set the viewport and scissor rectangle.
	D3D12_VIEWPORT viewport = _deviceResources->GetScreenViewport();
	_commandList->RSSetViewports(1, &viewport);
	_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Indicate this resource will be in use as a render target.
	CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_deviceResources->GetRenderTarget(), 
		D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	_commandList->ResourceBarrier(1, &renderTargetResourceBarrier);

	// Record drawing commands.
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = _deviceResources->GetRenderTargetView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = _deviceResources->GetDepthStencilView();
	_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
	_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	_commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

	// Bind the sampler to the pipeline.
	CD3DX12_GPU_DESCRIPTOR_HANDLE samplerGpuHandle(
		_samplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	_commandList->SetGraphicsRootDescriptorTable(2, samplerGpuHandle);

	//	Bind the scene constant buffer to the pipeline.
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle(
		_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	_commandList->SetGraphicsRootDescriptorTable(0, cbvGpuHandle);
}

void TurboApplicationDX12Renderer::PopulateCommandList(std::shared_ptr<IGameLevel> level)
{
	if (level == nullptr)
	{
		return;
	}

	auto scene = level->StaticScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			PopulateCommandList(sceneObject);
		}
	}

	scene = level->DynamicScene();
	if (scene != nullptr)
	{
		auto sceneObjects = scene->SceneObjects();

		for (auto& sceneObject : sceneObjects)
		{
			PopulateCommandList(sceneObject);
		}
	}
}

void TurboApplicationDX12Renderer::PopulateCommandList(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	D3D12_VERTEX_BUFFER_VIEW *pVertexBufferView = &_levelVertexBufferViews[sceneObject];
	D3D12_INDEX_BUFFER_VIEW *pIndexBufferView = &_levelIndexBufferViews[sceneObject];
	
	//int cbvOffset = _levelSceneObjectOffsets[sceneObject];
	//int cbvIndex = 1 + _deviceResources->GetCurrentFrameIndex() * _levelSceneObjectCount + cbvOffset;

	// Prepare to pass the updated model matrix to the shader.
	//XMStoreFloat4x4(&_constantBufferMappedResource[cbvIndex].model, XMMatrixIdentity());

	std::string textureName = sceneObject->Material()->Texture()->Name();
	
	int srvOffset = _levelTextureOffsets[textureName];
	int srvIndex = 1 + DX::c_frameCount * _levelSceneObjectCount + srvOffset;

	int indexCount = sceneObject->Mesh()->Triangles().size() * 3;

	//	Create handles for root signature arguments.
	//CD3DX12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle(
	//	_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
	//	cbvIndex,
	//	_cbvSrvDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(
		_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), 
		srvIndex,
		_cbvSrvDescriptorSize);

	//  Set Root Signature Arguments.
	//_commandList->SetGraphicsRootDescriptorTable(2, cbvGpuHandle);
	_commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_commandList->IASetVertexBuffers(0, 1, pVertexBufferView);
	_commandList->IASetIndexBuffer(pIndexBufferView);
	_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		PopulateCommandList(childSceneObject);
	}
}

void TurboApplicationDX12Renderer::FinalizeRendering()
{
	// Indicate that the render target will now be used to present when the command list is done executing.
	CD3DX12_RESOURCE_BARRIER presentResourceBarrier = 
		CD3DX12_RESOURCE_BARRIER::Transition(
			_deviceResources->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);

	_commandList->ResourceBarrier(1, &presentResourceBarrier);

	PIXEndEvent(_commandList.Get());

	ExecuteCommandList();

	_deviceResources->Present();
}

#pragma endregion

