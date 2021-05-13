#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <typename TArgs>
			class ITurboEventHandler
			{
			public:
				virtual void PublishEvent(void *sender, TArgs args) = 0;
			};
		}
	}
}