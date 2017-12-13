
#include <pch.h>

#include <ITurboImage.h>
#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>

#include <DirectX12Renderer.h>
#include <Windows10Helpers.h>

using namespace Concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::DirectX12;
using namespace Turbo::Platform::Windows10;

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
DirectX12Renderer::DirectX12Renderer(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService) :
	_debug(debug),
	_ioService(ioService),
	_constantBufferMappedResource(nullptr)
{
}

DirectX12Renderer::~DirectX12Renderer()
{
	ReleaseSceneResources();
}

#pragma endregion
#pragma region Methods -------------------------------------------------------------------------------------------------

void DirectX12Renderer::UpdateDisplayInformation()
{
	_deviceResources->UpdateDisplayInformation();
}

bool DirectX12Renderer::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
	ReleaseSceneResources();

	if (_deviceResources == nullptr)
	{
		_deviceResources = std::make_shared<DeviceResources>();
		_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
		_device = _deviceResources->GetD3DDevice();
	}

	CreateRootSignature();
	CreatePipelineStateObject();
	CreateCommandList();

	CreateSceneVertexResources(scene);
	CreateSceneTextureResources(scene);
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

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool DirectX12Renderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
	auto commandQueue = _deviceResources->GetCommandQueue();
	PIXBeginEvent(commandQueue, 0, L"Render");
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix(scene->CameraPlacement(), scene->LightHack());

		InitializeRendering();

		PopulateCommandList(scene);

		FinalizeRendering();
	}
	PIXEndEvent(commandQueue);

	return true;
}

#pragma endregion
#pragma region Local Support Methods -----------------------------------------------------------------------------------

void DirectX12Renderer::ReleaseSceneResources()
{
	if (_deviceResources != nullptr) 
	{
		_deviceResources->WaitForGpu();
	}

	if (_constantBufferMappedResource != nullptr)
	{
		_constantBufferTargetResource->Unmap(0, nullptr);
		_constantBufferMappedResource = nullptr;
		_constantBufferData = nullptr;
	}

	_sceneVertexTargetResources.clear();
	_sceneVertexSourceResources.clear();
	_sceneVertexBufferViews.clear();

	_sceneIndexTargetResources.clear();
	_sceneIndexSourceResources.clear();
	_sceneIndexBufferViews.clear();

	_sceneTextureTargetResources.clear();
	_sceneTextureSourceResources.clear();

	_sceneObjectOffsets.clear();
	_sceneObjectCount = 0;
	_sceneObjectMeshOffsets.clear();
	_sceneObjectMeshCount = 0;
	_sceneObjectTextureOffsets.clear();
	_sceneObjectTextureCount = 0;

	_commandList = nullptr;
	_rootSignature = nullptr;
	_pipelineState = nullptr;

	_cbvSrvDescriptorHeap = nullptr;
	_cbvSrvDescriptorSize = 0;
	_samplerDescriptorHeap = nullptr;

}

void DirectX12Renderer::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE descriptorRanges[4];
	descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
	descriptorRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descriptorRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	descriptorRanges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParameters[4];
	rootParameters[0].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[1].InitAsDescriptorTable(1, &descriptorRanges[1], D3D12_SHADER_VISIBILITY_VERTEX);
	rootParameters[2].InitAsDescriptorTable(1, &descriptorRanges[2], D3D12_SHADER_VISIBILITY_VERTEX);
	rootParameters[3].InitAsDescriptorTable(1, &descriptorRanges[3], D3D12_SHADER_VISIBILITY_PIXEL);

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

	ThrowIfFailed(D3D12SerializeRootSignature(
		&descRootSignature, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		pSignature.GetAddressOf(), 
		pError.GetAddressOf()));

	ThrowIfFailed(_device->CreateRootSignature(
		0, 
		pSignature->GetBufferPointer(), 
		pSignature->GetBufferSize(), 
		IID_PPV_ARGS(&_rootSignature)));
}

void DirectX12Renderer::CreatePipelineStateObject()
{
	std::vector<byte> vertexShaderData = _ioService->ReadData(L"DX12VertexShader.cso");
	std::vector<byte> pixelShaderData = _ioService->ReadData(L"DX12PixelShader.cso");


	static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.pRootSignature = _rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShaderData.data(), vertexShaderData.size());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShaderData.data(), pixelShaderData.size());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	ThrowIfFailed(_device->CreateGraphicsPipelineState(
		&psoDesc, 
		IID_PPV_ARGS(&_pipelineState)));

	// Shader data can be deleted once the pipeline state is created.
	vertexShaderData.clear();
	pixelShaderData.clear();
}

void DirectX12Renderer::CreateCommandList()
{
	ThrowIfFailed(_device->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		_deviceResources->GetCommandAllocator(), 
		_pipelineState.Get(), 
		IID_PPV_ARGS(&_commandList)));
}

void DirectX12Renderer::ResetCommandList()
{
	ThrowIfFailed(_deviceResources->GetCommandAllocator()->Reset());

	// The command list can be reset anytime after ExecuteCommandList() is called.
	ThrowIfFailed(_commandList->Reset(
		_deviceResources->GetCommandAllocator(), 
		_pipelineState.Get()));
}

void DirectX12Renderer::ExecuteCommandList()
{
	// Close the command list and execute it to begin the vertex/index buffer copy into the GPU's default heap.
	ThrowIfFailed(_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };

	auto commandQueue = _deviceResources->GetCommandQueue();
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

//	Scene Vertex Resources ---------------------------------------------------------------------------------------------

void DirectX12Renderer::CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	//	Prepare data structures.
	_sceneVertexTargetResources.clear();
	_sceneVertexSourceResources.clear();
	_sceneVertexBufferViews.clear();

	_sceneIndexTargetResources.clear();
	_sceneIndexSourceResources.clear();
	_sceneIndexBufferViews.clear();

	_sceneObjectMeshOffsets.clear();

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		LoadSceneObjectVertices(sceneObject);
	}

	_sceneObjectCount = (UINT)_sceneObjectOffsets.size();
	_sceneObjectMeshCount = (UINT)_sceneObjectMeshOffsets.size();
}

void DirectX12Renderer::LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject)
{

	UINT sceneObjectOffset = (UINT)_sceneObjectOffsets.size();
	_sceneObjectOffsets[sceneObject] = sceneObjectOffset;

	std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

	if (mesh == nullptr)
	{
		return;
	}

	//  Already loaded this mesh? don't reload it.
	if (_sceneObjectMeshOffsets.find(mesh) != _sceneObjectMeshOffsets.end())
	{
		return;
	}

	UINT meshOffset = (UINT)_sceneObjectMeshOffsets.size();
	_sceneObjectMeshOffsets[mesh] = meshOffset;

	std::vector<ShaderVertex> vertexList;
	std::vector<unsigned short> indexList;

	LoadVertexData(mesh, &vertexList, &indexList);

	//  Load mesh vertices ---------------------------------------------------------------------------------------------
	UINT vertexBufferSize = (UINT)(vertexList.size() * sizeof(vertexList[0]));

	// Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
	// The upload resource must not be released until after the GPU has finished using it.
	ComPtr<ID3D12Resource> vertexTargetResource;
	ComPtr<ID3D12Resource> vertexSourceResource;

	//  vertexTargetResource represents an allocated space in GPU memory
	//	for holding the vertex data.
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&vertexTargetResource)));

	//	vertexSourceResource represents an allocated space in CPU memory 
	//	for holding the vertex data to be uploaded to the GPU.
	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexSourceResource)));

	vertexTargetResource->SetName(L"Vertex Target Resource");
	vertexSourceResource->SetName(L"Vertex Source Resource");

	_sceneVertexTargetResources[mesh] = vertexTargetResource;
	_sceneVertexSourceResources[mesh] = vertexSourceResource;

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
		_sceneVertexBufferViews[mesh] = vertexBufferView;
	}

	//	Load mesh indices ----------------------------------------------------------------------------------------------
	UINT indexBufferSize = (UINT)(indexList.size() * sizeof(indexList[0]));

	// Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
	// The upload resource must not be released until after the GPU has finished using it.
	ComPtr<ID3D12Resource> indexTargetResource = nullptr;
	ComPtr<ID3D12Resource> indexSourceResource = nullptr;

	//  indexTargetResource represents an allocated space in GPU memory
	//	for holding the index data.
	CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&indexTargetResource)));

	//	indexSourceResource represents an allocated space in CPU memory 
	//	for holding the index data to be uploaded to the GPU.
	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexSourceResource)));

	indexTargetResource->SetName(L"Index Buffer Target Resource");
	indexSourceResource->SetName(L"Index Buffer Source Resource");

	_sceneIndexTargetResources[mesh] = indexTargetResource;
	_sceneIndexSourceResources[mesh] = indexSourceResource;

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
		_sceneIndexBufferViews[mesh] = indexBufferView;
	}


	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTextures(childSceneObject);
	}
}

void DirectX12Renderer::LoadVertexData(
	std::shared_ptr<ITurboSceneMesh> mesh,
	std::vector<ShaderVertex> *vertexList,
	std::vector<unsigned short> *indexList)
{
	for (auto& meshVertex : mesh->Vertices())
	{
		ShaderVertex shaderVertex;

		shaderVertex.Position = XMFLOAT3((float)(meshVertex.Position.X), (float)(meshVertex.Position.Y), (float)(meshVertex.Position.Z));
		shaderVertex.Normal = XMFLOAT3((float)(meshVertex.Normal.X), (float)(meshVertex.Normal.Y), (float)(meshVertex.Normal.Z));
		shaderVertex.Texture = XMFLOAT2((float)(meshVertex.TextureUV.X), (float)(meshVertex.TextureUV.Y));

		vertexList->push_back(shaderVertex);
	}

	for (auto& meshTriangle : mesh->Triangles())
	{
		indexList->push_back(meshTriangle.Vertex1);
		indexList->push_back(meshTriangle.Vertex2);
		indexList->push_back(meshTriangle.Vertex3);
	}
}

//	Scene Texture Resources --------------------------------------------------------------------------------------------

void DirectX12Renderer::CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	//	Prepare data structures.
	_sceneObjectTextureOffsets.clear();
	_sceneTextureTargetResources.clear();
	_sceneTextureSourceResources.clear();

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		LoadSceneObjectTextures(sceneObject);
	}

	_sceneObjectTextureCount = (UINT)_sceneObjectTextureOffsets.size();
}

void DirectX12Renderer::LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject)
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

	std::string textureName = texture->Name();

	//  Already loaded this texture? don't reload it.
	if (_sceneObjectTextureOffsets.find(textureName) != _sceneObjectTextureOffsets.end())
	{
		return;
	}

	UINT textureOffset = (UINT)_sceneObjectTextureOffsets.size();
	_sceneObjectTextureOffsets[textureName] = textureOffset;

	D3D12_RESOURCE_DESC textureResourceDesc = {};
	std::vector<unsigned char> textureData;

	LoadTextureData(textureName, &textureResourceDesc, &textureData);

	ComPtr<ID3D12Resource> textureTargetResource = nullptr;
	ComPtr<ID3D12Resource> textureSourceResource = nullptr;

	//  textureTargetResource represents an allocated space in GPU memory
	//	for holding the texture data.
	ThrowIfFailed(_device->CreateCommittedResource(
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
	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureSourceResource)));

	textureTargetResource->SetName(L"Texture Buffer Target Resource");
	textureSourceResource->SetName(L"Texture Buffer Source Resource");

	_sceneTextureTargetResources[textureName] = textureTargetResource;
	_sceneTextureSourceResources[textureName] = textureSourceResource;

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

void DirectX12Renderer::LoadTextureData(
	std::string textureName,
	D3D12_RESOURCE_DESC *textureResourceDesc,
	std::vector<unsigned char> *textureData)
{
	std::shared_ptr<ITurboCanvas> canvas = std::shared_ptr<ITurboCanvas>(new TurboCanvasRGBA32());
	std::vector<byte> fileData = _ioService->ReadData(ToWString(textureName + ".bmp"));
	std::shared_ptr<ITurboImage> bitmap = std::shared_ptr<ITurboImage>(new TurboBitmap(canvas.get(), fileData.data()));
	bitmap->Draw();

	unsigned char *canvasData = (unsigned char *)canvas->Data();
	int canvasDataSize = canvas->DataSize();
	textureData->assign(canvasData, canvasData + canvasDataSize);

	int textureWidth = canvas->Width();
	int textureHeight = canvas->Height();

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
}

//	Scene Assets -------------------------------------------------------------------------------------------------------

void DirectX12Renderer::CreateConstantBufferResources()
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
			(1 + c_frameCount * _sceneObjectCount) * c_alignedConstantBufferSize);
	
	ThrowIfFailed(_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_constantBufferTargetResource)));
	
	_constantBufferTargetResource->SetName(L"Constant Buffer Target Resource");
}

void DirectX12Renderer::CreateCBVSRVDescriptorHeap()
{
	//	Create a descriptor heap for the constant buffer resources (transform matrices) 
	//		and shader resources (textures).

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1 + c_frameCount * _sceneObjectCount + _sceneObjectTextureCount;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	This flag indicates that this descriptor heap can be bound to the 
	//		pipeline and that descriptors contained in it can be referenced 
	//		by a root table.
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&descriptorHeapDesc, 
		IID_PPV_ARGS(&_cbvSrvDescriptorHeap)));

	_cbvSrvDescriptorHeap->SetName(L"Constant Buffer View Descriptor Heap");

	_cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DirectX12Renderer::CreateCBVDescriptors()
{
	//	Create constant buffer view descriptors to access the upload buffer.
	//	The CBV descriptors point to locations in the CB heap resource created above.
	//	The CBV descriptors are added to the CBV/SRV descriptor heap.
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = _constantBufferTargetResource->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(
		_cbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT n = 0; n < 1 + c_frameCount * _sceneObjectCount; n++)
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
	ThrowIfFailed(_constantBufferTargetResource->Map(
		0, 
		&readRange,
		reinterpret_cast<void**>(&_constantBufferMappedResource)));
	
	ZeroMemory(_constantBufferMappedResource, (1 + c_frameCount * _sceneObjectCount) * c_alignedConstantBufferSize);
	// We don't unmap this until the app closes. Keeping things mapped for the lifetime of the resource is okay.
	_constantBufferData = (SceneConstantBuffer*)_constantBufferMappedResource;
}

void DirectX12Renderer::CreateSRVDescriptors()
{
	for (auto &sceneTextureOffset : _sceneObjectTextureOffsets)
	{
		std::string textureName = sceneTextureOffset.first;
		UINT textureOffset = sceneTextureOffset.second;
		auto sceneTextureTargetResource = _sceneTextureTargetResources[textureName];

		CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(
			_cbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1 + c_frameCount * _sceneObjectCount + textureOffset,
			_cbvSrvDescriptorSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		_device->CreateShaderResourceView(
			sceneTextureTargetResource.Get(), 
			&srvDesc, 
			srvCpuHandle);
	}
}

void DirectX12Renderer::CreateSamplerDescriptorHeap()
{
	// Describe and create a sampler descriptor heap.
	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
	samplerHeapDesc.NumDescriptors = 1;
	samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&samplerHeapDesc, 
		IID_PPV_ARGS(&_samplerDescriptorHeap)));
}

void DirectX12Renderer::CreateSamplerDescriptor()
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

void DirectX12Renderer::UpdateProjectionMatrix()
{
	// Initializes view parameters when the window size changes.

	Size outputSize = _deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = _deviceResources->GetScreenViewport();
	_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

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
		&_constantBufferData[0].Projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	// Prepare to pass the updated model matrix to the shader.
	//DirectX::XMMATRIX model = DirectX::XMMATRIX(
	//	1.0, 0.0, 0.0, 1.5,
	//	0.0, 1.0, 0.0,-1.5,
	//	0.0, 0.0, 1.0, 0.0,
	//	0.0, 0.0, 0.0, 1.0);
	//XMStoreFloat4x4(&_constantBufferMappedResource[0].model, model);

	//	XMStoreFloat4x4(&_constantBufferData[0].model, XMMatrixIdentity());
}

void DirectX12Renderer::UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack)
{
	TurboVector3D position = cameraPlacement->Position();
	TurboVector3D target = cameraPlacement->Target();
	TurboVector3D up = cameraPlacement->Up();
	TurboVector3D front = -cameraPlacement->Back();

	XMVECTORF32 eyePosition = { (float)(position.X), (float)(position.Y), (float)(position.Z), 1.0f };
	XMVECTORF32 focusPosition = { (float)(target.X), (float)(target.Y), (float)(target.Z), 1.0f };
	XMVECTORF32 upDirection = { (float)(up.X), (float)(up.Y), (float)(up.Z), 1.0f };
	XMVECTORF32 eyeDirection = { (float)(front.X), (float)(front.Y), (float)(front.Z), 1.0f };

	XMMATRIX lookAtRH = XMMatrixLookAtRH(eyePosition, focusPosition, upDirection);
	XMMATRIX transpose = XMMatrixTranspose(lookAtRH);

	XMVECTOR determinant = XMMatrixDeterminant(transpose);
	XMMATRIX inverse = XMMatrixInverse(&determinant, lookAtRH);

	XMStoreFloat4x4(&_constantBufferData->View, transpose);
	XMStoreFloat4x4(&_constantBufferData->ViewInverseTranspose, inverse);
	XMStoreFloat4(&_constantBufferData->Camera, eyeDirection);

	_constantBufferData->LightCount = lightHack ? 1 : 0;
}

void DirectX12Renderer::InitializeRendering()
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
	_commandList->RSSetScissorRects(1, &_scissorRect);

	// Indicate this resource will be in use as a render target.
	CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_deviceResources->GetRenderTarget(), 
		D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	_commandList->ResourceBarrier(1, &renderTargetResourceBarrier);

	// Record drawing commands.
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = _deviceResources->GetRenderTargetView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = _deviceResources->GetDepthStencilView();
//	_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
	_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::MidnightBlue, 0, nullptr);
//	_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::Yellow, 0, nullptr);
	_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	_commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

	// Bind the sampler to the pipeline.
	CD3DX12_GPU_DESCRIPTOR_HANDLE samplerGpuHandle(
		_samplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	_commandList->SetGraphicsRootDescriptorTable(0, samplerGpuHandle);

	//	Bind the scene constant buffer to the pipeline.
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle(
		_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	_commandList->SetGraphicsRootDescriptorTable(1, cbvGpuHandle);
}

void DirectX12Renderer::PopulateCommandList(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		PopulateCommandList(sceneObject);
	}
}

void DirectX12Renderer::PopulateCommandList(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

	if (mesh == nullptr)
	{
		return;
	}

	D3D12_VERTEX_BUFFER_VIEW *pVertexBufferView = &_sceneVertexBufferViews[mesh];
	D3D12_INDEX_BUFFER_VIEW *pIndexBufferView = &_sceneIndexBufferViews[mesh];
	
	int cbvOffset = _sceneObjectOffsets[sceneObject];
	int cbvIndex = 1 + _deviceResources->GetCurrentFrameIndex() * _sceneObjectCount + cbvOffset;

	// Prepare to pass the updated model matrix to the shader.
	TurboMatrix4x4 transform = sceneObject->Placement()->Transform();
	DirectX::XMMATRIX model = DirectX::XMMATRIX(
		(float)transform.M11, (float)transform.M12, (float)transform.M13, (float)transform.M14,
		(float)transform.M21, (float)transform.M22, (float)transform.M23, (float)transform.M24,
		(float)transform.M31, (float)transform.M32, (float)transform.M33, (float)transform.M34,
		(float)transform.M41, (float)transform.M42, (float)transform.M43, (float)transform.M44);

	//  Transpose because DirectX (and Wikipedia, apparently) 
	//	uses a transposed version compared to what I'm used to.
	//	Is it a LH vs. RH thing?
	XMMATRIX transpose = XMMatrixTranspose(model);

	XMStoreFloat4x4(&_constantBufferMappedResource[cbvIndex].Model, transpose);

	XMVECTOR determinant; // = XMMatrixDeterminant(transpose);
	XMMATRIX inverse = XMMatrixInverse(&determinant, model);

	XMStoreFloat4x4(&_constantBufferMappedResource[cbvIndex].ModelInverseTranspose, inverse);

	std::string textureName = sceneObject->Material()->Texture()->Name();
	
	int srvOffset = _sceneObjectTextureOffsets[textureName];
	int srvIndex = 1 + c_frameCount * _sceneObjectCount + srvOffset;

	UINT indexCount = (UINT)(sceneObject->Mesh()->Triangles().size() * 3);

	//	Create handles for root signature arguments.
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle(
		_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		cbvIndex,
		_cbvSrvDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(
		_cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), 
		srvIndex,
		_cbvSrvDescriptorSize);

	//  Set Root Signature Arguments.
	_commandList->SetGraphicsRootDescriptorTable(2, cbvGpuHandle);
	_commandList->SetGraphicsRootDescriptorTable(3, srvGpuHandle);

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

void DirectX12Renderer::FinalizeRendering()
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

