#pragma once

#include <pch.h>

#include <ITurboDebugLogger.h>
#include <TurboDebugStream.h>
#include <TurboDebugTypes.h>


namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class ITurboDebug
			{
			public:
				virtual void Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message) = 0;
				virtual TurboDebugStream& Send(TurboDebugSeverity severity, TurboDebugCategory category) = 0;

				virtual std::shared_ptr<ITurboDebugLogger> Logger() = 0;
				virtual void Logger(std::shared_ptr<ITurboDebugLogger> logger) = 0;
			};
		}
	}
}