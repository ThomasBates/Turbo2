#pragma once

ref class ApplicationDX12FrameworkViewSource /*sealed*/ : Windows::ApplicationModel::Core::IFrameworkViewSource
{
private:
	std::shared_ptr<IProgram> _program;

internal:
	//  Constructor(s)
	ApplicationDX12FrameworkViewSource();
	ApplicationDX12FrameworkViewSource(std::shared_ptr<IProgram> program);

public:
	//  IFrameworkViewSource Methods
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
