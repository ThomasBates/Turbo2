
#include <pch.h>
#include <iostream>
#include <fstream>
#include <string>

#include <TurboDebugTypes.h>
#include <AndroidNDKDebugLogCatLogger.h>
#include <android/log.h>

#define APPNAME "MazePreview"

//using namespace Platform;
using namespace Turbo::Platform::AndroidNDK;

void AndroidNDKDebugLogCatLogger::Log(std::string severity, std::string category, std::string message)
{
	__android_log_print(ANDROID_LOG_INFO, APPNAME, "%s - %s - %s", severity.data(), category.data(), message.data());
	return;
}
