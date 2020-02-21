
#include <TurboRelayCommand.h>

using namespace Turbo::Core::Presentation;

TurboRelayCommand::TurboRelayCommand(void (*executeCallback)(), bool (*canExecuteCallback)()) :
	_executeCallback(executeCallback),
	_canExecuteCallback(canExecuteCallback)
{
}
			
virtual bool TurboRelayCommand::CanExecute()
{
	return _canExecuteCallback();
}

virtual void TurboRelayCommand::Execute()
{
	if (!CanExecute())
		return;

	_executeCallback();
}

template <class T>
TurboRelayCommand<T>::TurboRelayCommand(void (*executeCallback)(T), bool (*canExecuteCallback)(T)) :
		_executeCallback(executeCallback),
		_canExecuteCallback(canExecuteCallback)
{
}

template <class T>
virtual bool TurboRelayCommand<T>::CanExecute(T parameter)
{
	return _canExecuteCallback(parameter);
}

template <class T>
virtual void TurboRelayCommand<T>::Execute(T parameter)
{
	if (!CanExecute(parameter))
		return;

	_executeCallback(parameter);
}
