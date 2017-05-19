
#pragma once

#include "ITurboGame.h"

namespace Application_DX12
{
	class DX12Game : public ITurboGame
	{
	public:
		//  ITurboGame Methods
		virtual BOOL Run(std::shared_ptr<ITurboGameLevel> level);
	};
}
