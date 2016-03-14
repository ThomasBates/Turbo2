#pragma once

#include "IDeviceResources.h"
#include "IApplicationDX12DeviceResources.h"

namespace Application_DX12
{
	class ApplicationDX12DeviceResources : public IDeviceResources, public IApplicationDX12DeviceResources
	{
	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

	public:
		//  Constructors and Destructors
		ApplicationDX12DeviceResources(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
			m_deviceResources(deviceResources)
		{
		};

		//  IDeviceResources Methods
		virtual void MakePolymorphic() {}

		//  IApplicationDX12DeviceResources Methods
		virtual std::shared_ptr<DX::DeviceResources> GetDeviceResources() { return m_deviceResources; }
	};

}