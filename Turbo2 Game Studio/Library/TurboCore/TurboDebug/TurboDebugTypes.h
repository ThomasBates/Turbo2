#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			enum TurboDebugSeverity
			{
				Error,
				Warning,
				Information
			};

			enum TurboDebugCategory
			{
				DebugLevel0
			};

			inline std::string DebugSeverity(TurboDebugSeverity severity)
			{
				switch (severity)
				{
				case Error: return "Error";
				case Warning: return "Warning";
				case Information: return "Information";
				}
				return "Unknown Severity";
			}

			inline std::string DebugCategory(TurboDebugCategory category)
			{
				switch (category)
				{
				case DebugLevel0: return "DebugLevel0";
				}
				return "Unknown Category";
			}
		}
	}
}