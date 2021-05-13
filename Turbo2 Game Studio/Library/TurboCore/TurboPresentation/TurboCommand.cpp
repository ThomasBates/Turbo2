
#include <pch.h>

#include <TurboCommand.h>

using namespace Turbo::Core::Presentation;

template <class TReceiver>
TurboCommand<TReceiver>::TurboCommand(TReceiver *receiver, ExecuteCallback executeCallback, CanExecuteCallback canExecuteCallback) :
	_receiver(receiver),
	_executeCallback(executeCallback),
	_canExecuteCallback(canExecuteCallback)
{
}

template <class TReceiver>
bool TurboCommand<TReceiver>::CanExecute()
{
	if (_receiver != nullptr && _canExecuteCallback != nullptr)
		return (_receiver->*_canExecuteCallback)();

	return true;
}

template <class TReceiver>
void TurboCommand<TReceiver>::Execute()
{
	if (!CanExecute())
		return;

	if (_receiver != nullptr && _executeCallback != nullptr)
		(_receiver->*_executeCallback)();
}
