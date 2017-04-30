
#pragma once

#include "IApplication.h"

namespace Application_DX12
{
	class ApplicationDX12 : public IApplication
	{
	public:
		//  IApplication Methods
		virtual BOOL Run(std::shared_ptr<IGameLevel> program);
	};
}
