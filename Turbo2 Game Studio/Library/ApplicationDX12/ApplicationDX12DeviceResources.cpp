#include "pch.h"

#include "ApplicationDX12DeviceResources.h"

using namespace SpaceLabyrinthDX12;

ApplicationDX12DeviceResources::ApplicationDX12DeviceResources(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}
