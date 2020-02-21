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
				//  Constructors & Destructors -------------------------------------------------------------------------
				AndroidNDKDebugLogCatLogger(std::string appName);
				virtual ~AndroidNDKDebugLogCatLogger(){}

				//  ITurboDebugLogger Methods --------------------------------------------------------------------------
				virtual void Log(std::string severity, std::string category, std::string message);

			private:
				std::string _appName;
				//char _fileName[512] = { 0 };
			};
		}
	}
}