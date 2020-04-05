
#include <pch.h>

#include <iostream>
#include <fstream>
#include <string>

#include <TurboGameState.h>
#include <AndroidNDKIOService.h>
#include <TurboCoreHelpers.h>
#include "JNIHelper.h"

using namespace Turbo::Core;
using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace ndk_helper;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKIOService::AndroidNDKIOService(std::shared_ptr<ITurboDebug> debug) :
	_debug(debug)
{
}

//	ITurboGameIOService Methods ----------------------------------------------------------------------------------------

void AndroidNDKIOService::SaveGameState(std::shared_ptr<ITurboGameState> gameState)
{
	if (gameState == nullptr)
	{
		return;
	}

	char* configFilePath = GetConfigFilePath();

	std::ofstream out(configFilePath, std::ios::out);
	if (out)
	{
		std::vector<std::string> keys = gameState->Keys();
		for (const auto& key : keys)
		{
			std::string value = gameState->LoadString(key, "");
			out << key << "=" << value << '\n';
		}
		out.close();
	}
}

std::shared_ptr<ITurboGameState> AndroidNDKIOService::LoadGameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());

	return gameState;
}

std::vector<unsigned char> AndroidNDKIOService::ReadData(const std::wstring &filename)
{
	std::vector<unsigned char> data; //  = std::vector<unsigned char>();

	std::string temp = ToString(filename);
	const char *fileName = temp.data();

	if (!JNIHelper::GetInstance()->ReadFile(fileName, &data))
	{
		_debug->Send(debugError, debugIOService) << "Can not open a file: " << fileName << "\n";
	}

	return data;
}

int AndroidNDKIOService::WriteData(const std::wstring &filename, std::vector<unsigned char> fileData)
{
	return 0;
}

//Concurrency::task<std::vector<unsigned char>> AndroidNDKIOService::ReadDataAsync(const std::wstring &filename)
//{
//	auto folder = Package::Current->InstalledLocation;
//
//	return create_task(folder->GetFileAsync(StringReference((wchar_t*)filename.c_str()))).then([=](StorageFile^ file)
//	{
//		return FileIO::ReadBufferAsync(file);
//	}).then([](IBuffer^ fileBuffer) -> std::vector<unsigned char>
//	{
//		std::vector<unsigned char> returnBuffer;
//		returnBuffer.resize(fileBuffer->Length);
//		DataReader::FromBuffer(fileBuffer)->ReadBytes(ArrayReference<unsigned char>(returnBuffer.data(), fileBuffer->Length));
//		return returnBuffer;
//	});
//}

//task<uint32> AndroidNDKIOService::WriteDataAsync(const std::wstring &filename, std::vector<unsigned char> fileData)
//{
//	return task<uint32>();
//}

std::wstring AndroidNDKIOService::GetFullPath(std::wstring filename)
{
//	WCHAR path[512];
//	UINT pathSize = _countof(path);
//
//	DWORD size = GetModuleFileName(nullptr, path, pathSize);
//	if (size == 0 || size == pathSize)
//	{
//		// Method failed or path was truncated.
//		throw std::exception();
//	}
//
//	WCHAR* lastSlash = wcsrchr(path, L'\\');
//	if (lastSlash)
//	{
//		*(lastSlash + 1) = L'\0';
//	}
//
//	std::wstring pathString = path;
//	pathString = pathString + filename;
//	return pathString;

	return L"";
}

char* AndroidNDKIOService::GetConfigFilePath()
{
//	if (_configFilePath[0] == 0)
//	{
//		//	Get Local Folder
//		String^ localfolderS = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
//		//	convert folder name from wchar to ascii
//		std::wstring localFolderW(localfolderS->Begin());
//		std::string localFolderA(localFolderW.begin(), localFolderW.end());
//		const char* localFolder = localFolderA.c_str();
//
//		TCHAR moduleFileNameT[MAX_PATH];
//		GetModuleFileName(NULL, moduleFileNameT, MAX_PATH);
//		//	convert folder name from wchar to ascii
//		std::wstring moduleFileNameW(moduleFileNameT);
//		std::string moduleFileNameA(moduleFileNameW.begin(), moduleFileNameW.end());
//		const char *moduleFileName = moduleFileNameA.c_str();
//
//		char fileDir[MAX_PATH] = {};
//		char fileName[MAX_PATH] = {};
//		char fileExt[MAX_PATH] = {};
//		DecomposePath(moduleFileName, fileDir, fileName, fileExt);
//
//
//		snprintf(_configFilePath, MAX_PATH, "%s\\%s.config", localFolder, fileName);
//	}

	return _configFilePath;
}

void AndroidNDKIOService::DecomposePath(const char *filePath, char *fileDir, char *fileName, char *fileExt)
{
//#if defined _WIN32
//	const char *lastSeparator = strrchr(filePath, '\\');
//#else
//	const char *lastSeparator = strrchr(filePath, '/');
//#endif
//
//	const char *lastDot = strrchr(filePath, '.');
//	const char *endOfPath = filePath + strlen(filePath);
//	const char *startOfName = lastSeparator ? lastSeparator + 1 : filePath;
//	const char *startOfExt = lastDot > startOfName ? lastDot : endOfPath;
//
//	if (fileDir)
//		_snprintf_s(fileDir, MAX_PATH, _TRUNCATE, "%.*s", startOfName - filePath, filePath);
//
//	if (fileName)
//		_snprintf_s(fileName, MAX_PATH, _TRUNCATE, "%.*s", startOfExt - startOfName, startOfName);
//
//	if (fileExt)
//		_snprintf_s(fileExt, MAX_PATH, _TRUNCATE, "%s", startOfExt);
}