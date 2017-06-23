#pragma once

#include <pch.h>

#include <ITurboDebugLogger.h>
#include <TurboDebugTypes.h>

using namespace Turbo::Core::Debug;

namespace Turbo
{
	namespace Platform
	{
		namespace Windows10
		{
			class Windows10DebugFileLogger : public ITurboDebugLogger
			{
			public:
				virtual void Log(std::string severity, std::string category, std::string message);

			private:
				char _fileName[512] = { 0 };
			};
		}
	}
}