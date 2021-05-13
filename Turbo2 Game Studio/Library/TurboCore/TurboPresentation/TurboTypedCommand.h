#pragma once

#include <pch.h>

#include <ITurboTypedCommand.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			template <class TReceiver, typename TParam>
			class TurboTypedCommand : public ITurboTypedCommand<TParam>
			{
			public:
				typedef void (TReceiver::*ExecuteCallback)(TParam);
				typedef bool (TReceiver::*CanExecuteCallback)(TParam);

				TurboTypedCommand(TReceiver *receiver, ExecuteCallback executeCallback, CanExecuteCallback canExecuteCallback);
				virtual ~TurboTypedCommand(){}

				virtual bool CanExecute(TParam parameter);
				virtual void Execute(TParam parameter);

			private:
				TReceiver *_receiver;
				ExecuteCallback _executeCallback;
				CanExecuteCallback _canExecuteCallback;
			};
		}
	}
}