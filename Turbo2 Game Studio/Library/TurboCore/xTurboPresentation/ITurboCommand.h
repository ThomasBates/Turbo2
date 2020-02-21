#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			class ITurboCommand
			{
			public:
				virtual bool CanExecute() = 0;
				virtual void Execute() = 0;
			};

			template <class T>
			class ITurboCommand
			{
			public:
				virtual bool CanExecute(T parameter) = 0;
				virtual void Execute(T parameter) = 0;
			};
		}
	}
}