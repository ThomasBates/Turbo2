
#include <pch.h>

#include <TurboTypedCommand.h>

using namespace Turbo::Core::Presentation;

template <class TReceiver, typename TParam>
TurboTypedCommand<TReceiver, TParam>::TurboTypedCommand(TReceiver *receiver, ExecuteCallback executeCallback, CanExecuteCallback canExecuteCallback) :
	_receiver(receiver),
	_executeCallback(executeCallback),
	_canExecuteCallback(canExecuteCallback)
{
}

template <class TReceiver, typename TParam>
bool TurboTypedCommand<TReceiver, TParam>::CanExecute(TParam parameter)
{
	if (_receiver != nullptr && _canExecuteCallback != nullptr)
		return (_receiver->*_canExecuteCallback)(parameter);

	return true;
}

template <class TReceiver, typename TParam>
void TurboTypedCommand<TReceiver, TParam>::Execute(TParam parameter)
{
	if (!CanExecute(parameter))
		return;

	if (_receiver != nullptr && _executeCallback != nullptr)
		(_receiver->*_executeCallback)(parameter);
}
