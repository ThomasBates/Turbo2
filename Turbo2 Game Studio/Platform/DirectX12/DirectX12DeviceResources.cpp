﻿#include <pch.h>
#include <DirectX12DeviceResources.h>
#include <Windows10Helpers.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;

using namespace Turbo::Platform::DirectX12;
using namespace Turbo::Platform::Windows10;

namespace DisplayMetrics
{
	// High resolution displays can require a lot of GPU and battery power to render.
	// High resolution phones, for example, may suffer from poor battery life if
	// games attempt to render at 60 frames per second at full fidelity.
	// The decision to render at full fidelity across all platforms and form factors
	// should be deliberate.
	static const bool SupportHighResolutions = false;

	// The default thresholds that define a "high resolution" display. If the thresholds
	// are exceeded and SupportHighResolutions is false, the dimensions will be scaled
	// by 50%.
	static const float DpiThreshold = 192.0f;		// 200% of standard desktop display.
	static const float WidthThreshold = 1920.0f;	// 1080p width.
	static const float HeightThreshold = 1080.0f;	// 1080p height.
};

// Constants used to calculate screen rotations.
namespace ScreenRotation
{
	// 0-degree Z-rotation
	static const XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 90-degree Z-rotation
	static const XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 180-degree Z-rotation
	static const XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 270-degree Z-rotation
	static const XMFLOAT4X4 Rotation270(
		0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
};

// Constructor for DirectX12DeviceResources.
DirectX12DeviceResources::DirectX12DeviceResources() :
	m_currentFrame(0),
	m_screenViewport(),
	m_rtvDescriptorSize(0),
	m_fenceEvent(0),
	m_d3dRenderTargetSize(),
	m_outputSize(),
	m_logicalSize(),
	m_nativeOrientation(DisplayOrientations::None),
	m_currentOrientation(DisplayOrientations::None),
	m_dpi(-1.0f),
	m_effectiveDpi(-1.0f),
	m_deviceRemoved(false)
{
	ZeroMemory(m_fenceValues, sizeof(m_fenceValues));
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}

// Configures resources that don't depend on the Direct3D device.
void DirectX12DeviceResources::CreateDeviceIndependentResources()
{
}

// Configures the Direct3D device, and stores handles to it and the device context.
void DirectX12DeviceResources::CreateDeviceResources()
{
#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));

	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(m_dxgiFactory.Get(), &adapter);

	// Create the Direct3D 12 API device object
	HRESULT hr = D3D12CreateDevice(
		adapter.Get(),					// The hardware adapter.
		D3D_FEATURE_LEVEL_11_0,			// Minimum feature level this app can support.
		IID_PPV_ARGS(&m_d3dDevice)		// Returns the Direct3D device created.
		);

	if (FAILED(hr))
	{
		// If the initialization fails, fall back to the WARP device.
		// For more information on WARP, see: 
		// http://go.microsoft.com/fwlink/?LinkId=286690

		ComPtr<IDXGIAdapter> warpAdapter;
		m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)); 

		ThrowIfFailed(
			D3D12CreateDevice(
				warpAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_d3dDevice)
				)
			);
	}

	// Create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	//  For three rotating frames, each one needs its own command allocator.
	for (UINT n = 0; n < c_frameCount; n++)
	{
		ThrowIfFailed(
			m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n]))
			);
	}

	// Create synchronization objects.
	ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceValues[m_currentFrame]++;

	m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
}

// These resources need to be recreated every time the window size is changed.
void DirectX12DeviceResources::CreateWindowSizeDependentResources()
{
	// Wait until all previous GPU work is complete.
	WaitForGpu();

	// Clear the previous window size specific content.
	//  The three rotating frames each have their own render target resources, 
	//	and each has a render target view descriptor on the rtv heap.
	for (UINT n = 0; n < c_frameCount; n++)
	{
		m_renderTargets[n] = nullptr;
	}
	m_rtvHeap = nullptr;

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	UpdateRenderTargetSize();

	// The width and height of the swap chain must be based on the window's
	// natively-oriented width and height. If the window is not in the native
	// orientation, the dimensions must be reversed.
	DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

	bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
	m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

	if (m_swapChain != nullptr)
	{
		// If the swap chain already exists, resize it.
		HRESULT hr = m_swapChain->ResizeBuffers(
			c_frameCount,
			lround(m_d3dRenderTargetSize.Width),
			lround(m_d3dRenderTargetSize.Height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			m_deviceRemoved = true;

			// Do not continue execution of this method. DirectX12DeviceResources will be destroyed and re-created.
			return;
		}
		else
		{
			ThrowIfFailed(hr);
		}
	}
	else
	{
		// Otherwise, create a new one using the same adapter as the existing Direct3D device.
		DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = lround(m_d3dRenderTargetSize.Width);	// Match the size of the window.
		swapChainDesc.Height = lround(m_d3dRenderTargetSize.Height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			// This is the most common swap chain format.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;							// Don't use multi-sampling.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = c_frameCount;					// Use triple-buffering to minimize latency.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// All Windows Universal apps must use _FLIP_ SwapEffects
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = scaling;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(
			m_dxgiFactory->CreateSwapChainForCoreWindow(
				m_commandQueue.Get(),								// Swap chains need a reference to the command queue in DirectX 12.
				reinterpret_cast<IUnknown*>(m_window.Get()),
				&swapChainDesc,
				nullptr,
				&swapChain
				)
			);

		ThrowIfFailed(swapChain.As(&m_swapChain));
	}

	// Set the proper orientation for the swap chain, and generate
	// 3D matrix transformations for rendering to the rotated swap chain.
	// The 3D matrix is specified explicitly to avoid rounding errors.

	switch (displayRotation)
	{
	case DXGI_MODE_ROTATION_IDENTITY:
		m_orientationTransform3D = ScreenRotation::Rotation0;
		break;

	case DXGI_MODE_ROTATION_ROTATE90:
		m_orientationTransform3D = ScreenRotation::Rotation270;
		break;

	case DXGI_MODE_ROTATION_ROTATE180:
		m_orientationTransform3D = ScreenRotation::Rotation180;
		break;

	case DXGI_MODE_ROTATION_ROTATE270:
		m_orientationTransform3D = ScreenRotation::Rotation90;
		break;

	default:
		throw ref new FailureException();
	}

	ThrowIfFailed(
		m_swapChain->SetRotation(displayRotation)
		);

	// Create render target views of the swap chain back buffer.
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = c_frameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
		m_rtvHeap->SetName(L"Render Target View Descriptor Heap");

		m_currentFrame = 0;

		//	Point rtvDescriptor to the first slot in the rtv heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (UINT n = 0; n < c_frameCount; n++)
		{
			//  Assign m_renderTargets[n] to reference the render target buffer resource within the swap chain.
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));

			//	Create a rtv descriptor and store it in the current slot in the rtv heap.
			m_d3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvDescriptor);

			//	Point rtvDescriptor to the next slot in the rtv heap.
			rtvDescriptor.Offset(m_rtvDescriptorSize);

			WCHAR name[25];
			swprintf_s(name, L"Render Target %d", n);
			m_renderTargets[n]->SetName(name);
		}
	}

	// Create a depth stencil view.
	{
		//	Create the depth stensil view (dsv) heap.
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

		//	Set properties to describe the depth stencil resource.
		D3D12_HEAP_PROPERTIES depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			static_cast<UINT>(m_d3dRenderTargetSize.Width),
			static_cast<UINT>(m_d3dRenderTargetSize.Height),
			1,
			0,
			1,
			0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
		depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
		depthOptimizedClearValue.DepthStencil.Stencil = 0;

		//	Create the depth stensil resource.
		ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
			&depthHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthOptimizedClearValue,
			IID_PPV_ARGS(&m_depthStencil)
			));

		//	Create the depth stencil view descriptor in the first (and only) slot of the dsv heap,
		//	pointing to the depth stencil resource.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// All pending GPU work was already finished. Update the tracked fence values
	// to the last value signaled.
	for (UINT n = 0; n < c_frameCount; n++)
	{
		m_fenceValues[n] = m_fenceValues[m_currentFrame];
	}

	// Set the 3D rendering viewport to target the entire window.
	m_screenViewport = { 0.0f, 0.0f, m_d3dRenderTargetSize.Width, m_d3dRenderTargetSize.Height, 0.0f, 1.0f };
}

// Determine the dimensions of the render target and whether it will be scaled down.
void DirectX12DeviceResources::UpdateRenderTargetSize()
{
	m_effectiveDpi = m_dpi;

	// To improve battery life on high resolution devices, render to a smaller render target
	// and allow the GPU to scale the output when it is presented.
	if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
	{
		float width = ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
		float height = ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

		if (width > DisplayMetrics::WidthThreshold && height > DisplayMetrics::HeightThreshold)
		{
			// To scale the app we change the effective DPI. Logical size does not change.
			m_effectiveDpi /= 2.0f;
		}
	}

	// Calculate the necessary render target size in pixels.
	m_outputSize.Width = ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
	m_outputSize.Height = ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

	// Prevent zero size DirectX content from being created.
	m_outputSize.Width = max(m_outputSize.Width, 1);
	m_outputSize.Height = max(m_outputSize.Height, 1);
}

// This method is called when the CoreWindow is created (or re-created).
void DirectX12DeviceResources::SetWindow(CoreWindow^ window)
{
	m_window = window;

	UpdateDisplayInformation();
}

void DirectX12DeviceResources::UpdateDisplayInformation()
{
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	m_logicalSize = Windows::Foundation::Size(m_window->Bounds.Width, m_window->Bounds.Height);
	m_nativeOrientation = currentDisplayInformation->NativeOrientation;
	m_currentOrientation = currentDisplayInformation->CurrentOrientation;
	m_dpi = currentDisplayInformation->LogicalDpi;

	CreateWindowSizeDependentResources();
}

// This method is called in the event handler for the SizeChanged event.
void DirectX12DeviceResources::SetLogicalSize(Windows::Foundation::Size logicalSize)
{
	if (m_logicalSize != logicalSize)
	{
		m_logicalSize = logicalSize;
		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the DpiChanged event.
void DirectX12DeviceResources::SetDpi(float dpi)
{
	if (dpi != m_dpi)
	{
		m_dpi = dpi;

		// When the display DPI changes, the logical size of the window (measured in Dips) also changes and needs to be updated.
		m_logicalSize = Windows::Foundation::Size(m_window->Bounds.Width, m_window->Bounds.Height);

		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the OrientationChanged event.
void DirectX12DeviceResources::SetCurrentOrientation(DisplayOrientations currentOrientation)
{
	if (m_currentOrientation != currentOrientation)
	{
		m_currentOrientation = currentOrientation;
		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the DisplayContentsInvalidated event.
void DirectX12DeviceResources::ValidateDevice()
{
	// The D3D Device is no longer valid if the default adapter changed since the device
	// was created or if the device has been removed.

	// First, get the LUID for the adapter from when the device was created.

	LUID previousAdapterLuid = m_d3dDevice->GetAdapterLuid();

	// Next, get the information for the current default adapter.

	ComPtr<IDXGIFactory2> currentFactory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

	ComPtr<IDXGIAdapter1> currentDefaultAdapter;
	ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

	DXGI_ADAPTER_DESC currentDesc;
	ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.

	if (previousAdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		previousAdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(m_d3dDevice->GetDeviceRemovedReason()))
	{
		m_deviceRemoved = true;
	}
}

// Present the contents of the swap chain to the screen.
void DirectX12DeviceResources::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was removed either by a disconnection or a driver upgrade, we 
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		m_deviceRemoved = true;
	}
	else
	{
		ThrowIfFailed(hr);

		MoveToNextFrame();
	}
}

// Wait for pending GPU work to complete.
void DirectX12DeviceResources::WaitForGpu()
{
	// Schedule a Signal command in the queue.
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_currentFrame]));

	// Wait until the fence has been crossed.
	ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	m_fenceValues[m_currentFrame]++;
}

// Prepare to render the next frame.
void DirectX12DeviceResources::MoveToNextFrame()
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// Advance the frame index.
	m_currentFrame = (m_currentFrame + 1) % c_frameCount;

	// Check to see if the next frame is ready to start.
	if (m_fence->GetCompletedValue() < m_fenceValues[m_currentFrame])
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	m_fenceValues[m_currentFrame] = currentFenceValue + 1;
}

// This method determines the rotation between the display device's native Orientation and the
// current display orientation.
DXGI_MODE_ROTATION DirectX12DeviceResources::ComputeDisplayRotation()
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	// Note: NativeOrientation can only be Landscape or Portrait even though
	// the DisplayOrientations enum has other values.
	switch (m_nativeOrientation)
	{
	case DisplayOrientations::Landscape:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case DisplayOrientations::Portrait:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}
	return rotation;
}

// This method acquires the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void DirectX12DeviceResources::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}
