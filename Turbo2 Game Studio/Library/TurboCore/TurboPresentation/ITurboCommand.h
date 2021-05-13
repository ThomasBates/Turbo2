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
		}
	}
}