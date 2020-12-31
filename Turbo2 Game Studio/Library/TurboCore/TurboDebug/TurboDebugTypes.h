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
				debugVerbose,
				debugDebug,
				debugInformation,
				debugWarning,
				debugError
			};

			inline std::string DebugSeverity(TurboDebugSeverity severity)
			{
				switch (severity)
				{
					case debugVerbose:		return "Verbose";
					case debugDebug:		return "Debug";
					case debugInformation:	return "Information";
					case debugWarning:		return "Warning";
					case debugError:		return "Error";
				}
				return "Unknown Debug Severity";
			}

			//	Debug Category -----------------------------------------------------------------------------------------

			enum TurboDebugCategory
			{
				debugApplication,
				debugStartup,
				debugController,
				debugInteractions,
				debugIOService,
				debugRenderer,
				debugAudio,
				debugView,
			};

			inline std::string DebugCategory(TurboDebugCategory category)
			{
				switch (category)
				{
					case debugApplication: return "Application";
					case debugStartup: return "Startup";
					case debugController: return "Controller";
					case debugInteractions: return "Interactions";
					case debugIOService: return "IOService";
					case debugRenderer: return "Renderer";
					case debugAudio: return "Audio";
					case debugView: return "View";
				}
				return "Unknown Debug Category";
			}
		}
	}
}