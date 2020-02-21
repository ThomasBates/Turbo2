#pragma once

#include <pch.h>

#include <ITurboCommand.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			class TurboRelayCommand : public ITurboCommand
			{
			public:
				TurboRelayCommand(void (*executeCallback)(), bool (*canExecuteCallback)());
			
				virtual bool CanExecute();
				virtual void Execute();
				
			private:
				void (*_executeCallback)();
				bool (*_canExecuteCallback)();
			};

			template <class T>
			class TurboRelayCommand<T> : public ITurboCommand<T>
			{
			public:
				TurboRelayCommand(void (*executeCallback)(T), bool (*canExecuteCallback)(T));

				virtual bool CanExecute(T parameter);
				virtual void Execute(T parameter);

			private:
				void (*_executeCallback)(T);
				bool (*_canExecuteCallback)(T);
			};
		}
	}
}