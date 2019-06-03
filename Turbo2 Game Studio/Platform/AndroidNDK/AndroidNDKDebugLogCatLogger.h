#pragma once

#include <pch.h>

#include <ITurboDebugLogger.h>
#include <TurboDebugTypes.h>

using namespace Turbo::Core::Debug;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			class AndroidNDKDebugLogCatLogger : public ITurboDebugLogger
			{
			public:
				AndroidNDKDebugLogCatLogger(){}
				virtual ~AndroidNDKDebugLogCatLogger(){}
				virtual void Log(std::string severity, std::string category, std::string message);

			private:
				//char _fileName[512] = { 0 };
			};
		}
	}
}