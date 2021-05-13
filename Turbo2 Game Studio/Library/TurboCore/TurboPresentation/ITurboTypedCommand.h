#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			template <class TParam>
			class ITurboTypedCommand
			{
			public:
				virtual bool CanExecute(TParam parameter) = 0;
				virtual void Execute(TParam parameter) = 0;
			};
		}
	}
}