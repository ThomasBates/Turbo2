#pragma once

#include "IPlatformResources.h"
#include "IApplicationDX12PlatformResources.h"

namespace Application_DX12
{
	class ApplicationDX12PlatformResources : public IPlatformResources, public IApplicationDX12PlatformResources
	{
	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

	public:
		//  Constructors and Destructors
		ApplicationDX12PlatformResources(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
			m_deviceResources(deviceResources)
		{
		};

		//  IPlatformResources Methods
		virtual void MakePolymorphic() {}

		//  IApplicationDX12DeviceResources Methods
		virtual std::shared_ptr<DX::DeviceResources> GetDeviceResources() { return m_deviceResources; }
	};

}