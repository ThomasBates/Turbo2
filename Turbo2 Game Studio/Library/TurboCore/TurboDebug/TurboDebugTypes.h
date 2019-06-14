#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			//	Debug Severity -----------------------------------------------------------------------------------------

			enum TurboDebugSeverity
			{
				debugError,
				debugWarning,
				debugInformation,
				debugDebug,
				debugVerbose
			};

			inline std::string DebugSeverity(TurboDebugSeverity severity)
			{
				switch (severity)
				{
					case debugError:		return "Error";
					case debugWarning:		return "Warning";
					case debugInformation:	return "Information";
					case debugDebug:		return "Debug";
					case debugVerbose:		return "Verbose";
				}
				return "Unknown Debug Severity";
			}

			//	Debug Category -----------------------------------------------------------------------------------------

			enum TurboDebugCategory
			{
				debugStartup,
				debugController,
				debugInteractions,
				debugIOService
			};

			inline std::string DebugCategory(TurboDebugCategory category)
			{
				switch (category)
				{
					case debugStartup: return "Startup";
					case debugController: return "Controller";
					case debugInteractions: return "Interactions";
					case debugIOService: return "IOService";
				}
				return "Unknown Debug Category";
			}
		}
	}
}