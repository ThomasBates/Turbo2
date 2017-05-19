#include "pch.h"

#include "DX12Game.h"
#include "DX12FrameworkViewSource.h"

using namespace Windows::ApplicationModel::Core;
using namespace Application_DX12;

#pragma region ITurboGame Methods

BOOL DX12Game::Run(std::shared_ptr<ITurboGameLevel> program)
{
	IFrameworkViewSource^ viewSource = ref new DX12FrameworkViewSource(program);
	CoreApplication::Run(viewSource);
	return 0;
}

#pragma endregion
