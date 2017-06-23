#pragma once

#include <pch.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

#include <TurboDebugTypes.h>
#include <Windows10DebugFileLogger.h>

using namespace Platform;
using namespace Turbo::Platform::Windows10;

void Windows10DebugFileLogger::Log(std::string severity, std::string category, std::string message)
{
	return;

	SYSTEMTIME lt;
	GetLocalTime(&lt);

	if (_fileName[0] == 0)
	{
		//	for local saving for future
		String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;

		//	convert folder name from wchar to ascii
		std::wstring folderNameW(localfolder->Begin());
		std::string folderNameA(folderNameW.begin(), folderNameW.end());
		const char* charStr = folderNameA.c_str();
		snprintf(_fileName, 512, "%s\\Turbo.%04d%02d%02d.%02d%02d%02d.Debug.txt", 
			charStr, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
	}

	char timeStamp[30];
	snprintf(timeStamp, 30, "%04d-%02d-%02d %02d:%02d:%02d.%03d ", 
		lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

	//write file
	{
		std::ofstream out(_fileName, std::ios::app);
		if (out)
		{
			out << timeStamp << category << ": " << message << std::endl;
			out.close();
		}
	}
}
