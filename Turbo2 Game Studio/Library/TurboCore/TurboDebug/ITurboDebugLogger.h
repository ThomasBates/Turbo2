#pragma once

#include <pch.h>
#include <TurboDebugTypes.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class ITurboDebugLogger
			{
			public:
				virtual void Log(std::string severity, std::string category, std::string message) = 0;
			};
		}
	}
}