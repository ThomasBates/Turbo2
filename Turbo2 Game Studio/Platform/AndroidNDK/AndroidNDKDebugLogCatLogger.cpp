
#include <pch.h>

#include <android/log.h>

#include <AndroidNDKDebugLogCatLogger.h>

using namespace Turbo::Platform::AndroidNDK;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

AndroidNDKDebugLogCatLogger::AndroidNDKDebugLogCatLogger(std::string appName) :
	_appName(appName)
{
}

//  ITurboDebugLogger Methods ------------------------------------------------------------------------------------------

void AndroidNDKDebugLogCatLogger::Log(std::string severity, std::string category, std::string message)
{
	__android_log_print(ANDROID_LOG_DEBUG, _appName.data(), "TurboDebug: %s - %s - %s", severity.data(), category.data(), message.data());
}
