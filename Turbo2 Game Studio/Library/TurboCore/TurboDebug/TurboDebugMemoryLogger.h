#pragma once

#include <pch.h>

#include <ITurboDebugLogger.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class TurboDebugMemoryLogger : public ITurboDebugLogger
			{
			public:
				//  ITurboDebugLogger Methods --------------------------------------------------------------------------
				virtual void Log(std::string severity, std::string category, std::string message);

			private:
				std::vector<std::string> _debugLog;
			};
		}
	}
}