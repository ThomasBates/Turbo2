#pragma once

ref class DX12FrameworkViewSource /*sealed*/ : Windows::ApplicationModel::Core::IFrameworkViewSource
{
private:
	std::shared_ptr<ITurboGameLevel> _level;

internal:
	//  Constructor(s)
	DX12FrameworkViewSource();
	DX12FrameworkViewSource(std::shared_ptr<ITurboGameLevel> program);

public:
	//  IFrameworkViewSource Methods
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
