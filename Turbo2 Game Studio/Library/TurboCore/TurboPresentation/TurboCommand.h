#pragma once

#include <pch.h>

#include <ITurboCommand.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			template <class TReceiver>
			class TurboCommand : public ITurboCommand
			{
			public:
				typedef void (TReceiver::*ExecuteCallback)();
				typedef bool (TReceiver::*CanExecuteCallback)();

				TurboCommand(TReceiver *receiver, ExecuteCallback executeCallback, CanExecuteCallback canExecuteCallback);
				virtual ~TurboCommand(){}

				virtual bool CanExecute();
				virtual void Execute();
				
			private:
				TReceiver *_receiver;
				ExecuteCallback _executeCallback;
				CanExecuteCallback _canExecuteCallback;
			};
		}
	}
}