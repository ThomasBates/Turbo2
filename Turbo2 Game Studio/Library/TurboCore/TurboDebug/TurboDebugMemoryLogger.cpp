
#include <pch.h>

#include <TurboDebugTypes.h>
#include <TurboDebugMemoryLogger.h>

using namespace Turbo::Core::Debug;

//  ITurboDebugLogger Methods ------------------------------------------------------------------------------------------

void TurboDebugMemoryLogger::Log(std::string severity, std::string category, std::string message)
{
	_debugLog.push_back(message);
}
