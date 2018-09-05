#pragma once

#include <pch.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;

namespace Turbo 
{
	namespace Platform
	{
		namespace DirectX12
		{
			static const UINT c_frameCount = 3;		// Use triple buffering.

			// Controls all the DirectX device resources.
			class DirectX12DeviceResources
			{
			public:
				DirectX12DeviceResources();
				void SetWindow(CoreWindow^ window);
				void UpdateDisplayInformation();
				void SetLogicalSize(Size logicalSize);
				void SetCurrentOrientation(DisplayOrientations currentOrientation);
				void SetDpi(float dpi);
				void ValidateDevice();
				void Present();
				void WaitForGpu();

				// The size of the render target, in pixels.
				Size GetOutputSize() const { return m_outputSize; }

				// The size of the render target, in dips.
				Size GetLogicalSize() const { return m_logicalSize; }

				float GetDpi() const { return m_effectiveDpi; }
				bool IsDeviceRemoved() const { return m_deviceRemoved; }

				// D3D Accessors.
				ID3D12Device*			GetD3DDevice() const { return m_d3dDevice.Get(); }
				IDXGISwapChain3*		GetSwapChain() const { return m_swapChain.Get(); }
				ID3D12Resource*			GetRenderTarget() const { return m_renderTargets[m_currentFrame].Get(); }
				ID3D12Resource*			GetDepthStencil() const { return m_depthStencil.Get(); }
				ID3D12CommandQueue*		GetCommandQueue() const { return m_commandQueue.Get(); }
				ID3D12CommandAllocator*	GetCommandAllocator() const { return m_commandAllocators[m_currentFrame].Get(); }
				D3D12_VIEWPORT			GetScreenViewport() const { return m_screenViewport; }
				DirectX::XMFLOAT4X4		GetOrientationTransform3D() const { return m_orientationTransform3D; }
				UINT					GetCurrentFrameIndex() const { return m_currentFrame; }

				CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
				{
					return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
				}
				CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
				{
					return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
				}

			private:
				void CreateDeviceIndependentResources();
				void CreateDeviceResources();
				void CreateWindowSizeDependentResources();
				void UpdateRenderTargetSize();
				void MoveToNextFrame();
				DXGI_MODE_ROTATION ComputeDisplayRotation();
				void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

				UINT							m_currentFrame;

				// Direct3D objects.
				ComPtr<ID3D12Device>			m_d3dDevice = nullptr;
				ComPtr<IDXGIFactory4>			m_dxgiFactory = nullptr;
				ComPtr<IDXGISwapChain3>			m_swapChain = nullptr;
				ComPtr<ID3D12Resource>			m_renderTargets[c_frameCount];
				ComPtr<ID3D12Resource>			m_depthStencil = nullptr;
				ComPtr<ID3D12DescriptorHeap>	m_rtvHeap = nullptr;
				ComPtr<ID3D12DescriptorHeap>	m_dsvHeap = nullptr;
				UINT							m_rtvDescriptorSize;
				ComPtr<ID3D12CommandQueue>		m_commandQueue = nullptr;
				ComPtr<ID3D12CommandAllocator>	m_commandAllocators[c_frameCount];
				D3D12_VIEWPORT					m_screenViewport;
				bool							m_deviceRemoved;

				// CPU/GPU Synchronization.
				ComPtr<ID3D12Fence>				m_fence = nullptr;
				UINT64							m_fenceValues[c_frameCount];
				HANDLE							m_fenceEvent;

				// Cached reference to the Window.
				Agile<CoreWindow>				m_window;

				// Cached device properties.
				Size							m_d3dRenderTargetSize;
				Size							m_outputSize;
				Size							m_logicalSize;
				DisplayOrientations				m_nativeOrientation;
				DisplayOrientations				m_currentOrientation;
				float							m_dpi;

				// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
				float							m_effectiveDpi;

				// Transforms used for display orientation.
				XMFLOAT4X4						m_orientationTransform3D;
			};
		}
	}
}