#pragma once

ref class SpaceLabyrinthDX12FrameworkViewSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
