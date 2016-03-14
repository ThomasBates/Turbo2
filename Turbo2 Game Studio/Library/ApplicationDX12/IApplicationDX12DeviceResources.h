#pragma once

#include "pch.h"

#include "Common\DeviceResources.h"

namespace Application_DX12
{
	class IApplicationDX12DeviceResources
	{
	public:
		virtual std::shared_ptr<DX::DeviceResources> GetDeviceResources() = 0;
	};
}