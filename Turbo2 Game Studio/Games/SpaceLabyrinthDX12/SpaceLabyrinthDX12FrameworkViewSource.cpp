#include "pch.h"

#include "IProgram.h"
#include "SpaceLabyrinthDX12FrameworkView.h"
#include "SpaceLabyrinthDX12FrameworkViewSource.h"

using namespace SpaceLabyrinthDX12;

using namespace Windows::ApplicationModel::Core;

IFrameworkView^ SpaceLabyrinthDX12FrameworkViewSource::CreateView()
{
	return ref new SpaceLabyrinthDX12FrameworkView();
}
