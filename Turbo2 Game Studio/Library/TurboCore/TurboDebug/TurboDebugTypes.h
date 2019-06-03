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
				debugInformation
			};

			inline std::string DebugSeverity(TurboDebugSeverity severity)
			{
				switch (severity)
				{
					case debugError:		return "Error";
					case debugWarning:		return "Warning";
					case debugInformation:	return "Information";
				}
				return "Unknown Debug Severity";
			}

			//	Debug Category -----------------------------------------------------------------------------------------

			enum TurboDebugCategory
			{
				debugStartup,
				debugInteractions
			};

			inline std::string DebugCategory(TurboDebugCategory category)
			{
				switch (category)
				{
					case debugStartup: return "Startup";
					case debugInteractions: return "Interactions";
				}
				return "Unknown Debug Category";
			}
		}
	}
}