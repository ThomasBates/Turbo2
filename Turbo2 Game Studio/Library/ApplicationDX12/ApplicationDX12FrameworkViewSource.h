#pragma once

ref class ApplicationDX12FrameworkViewSource /*sealed*/ : Windows::ApplicationModel::Core::IFrameworkViewSource
{
private:
	IProgram *_program;

internal:
	//  Constructor(s)
	ApplicationDX12FrameworkViewSource();
	ApplicationDX12FrameworkViewSource(IProgram *program);

public:
	//  IFrameworkViewSource Methods
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
