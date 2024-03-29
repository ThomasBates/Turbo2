
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
				//  ITurboDebug Properties -----------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboDebugLogger> Logger() = 0;
				virtual void Logger(std::shared_ptr<ITurboDebugLogger> logger) = 0;

				virtual TurboDebugSeverity Severity() = 0;
				virtual void Severity(TurboDebugSeverity severity) = 0;

				virtual bool CategoryEnabled(TurboDebugCategory category) = 0;
				virtual void CategoryEnabled(TurboDebugCategory category, bool enabled) = 0;

				//  ITurboDebug Methods --------------------------------------------------------------------------------
				virtual void Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message) = 0;
				virtual TurboDebugStream& Send(TurboDebugSeverity severity, TurboDebugCategory category) = 0;
			};
		}
	}
}