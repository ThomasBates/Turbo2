#include <pch.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

#include <TurboGameState.h>
#include <Windows10IOService.h>
#include <Windows10Helpers.h>

using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::Windows10;

Windows10IOService::Windows10IOService(std::shared_ptr<ITurboDebug> debug) :
	_debug(debug)
{
}

void Windows10IOService::SaveGameState(std::shared_ptr<ITurboGameState> gameState)
{
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

std::shared_ptr<ITurboGameState> Windows10IOService::LoadGameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());

	char* configFilePath = GetConfigFilePath();

	std::ifstream in(configFilePath);
	if (in)
	{
		std::string line;
		while (std::getline(in, line))
		{
			std::istringstream in(line);
			std::string key;
			if (std::getline(in, key, '='))
			{
				std::string value;
				if (std::getline(in, value))
				{
					gameState->SaveString(key, value);
				}
			}
		}
	}
	return gameState;
}

std::vector<byte> Windows10IOService::ReadData(const std::wstring &filename)
{
	std::wstring fullPath = GetFullPath(filename);

	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	Wrappers::FileHandle file(
		CreateFile2(
			fullPath.c_str(),
			GENERIC_READ, 
			FILE_SHARE_READ, 
			OPEN_EXISTING, 
			&extendedParams));

	if (file.Get() == INVALID_HANDLE_VALUE)
	{
		throw std::exception();
	}

	FILE_STANDARD_INFO fileInfo = {0};
	if (!GetFileInformationByHandleEx(
		file.Get(), 
		FileStandardInfo, 
		&fileInfo, 
		sizeof(fileInfo)))
	{
		throw std::exception();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw std::exception();
	}

	std::vector<byte> outputBuffer = std::vector<byte>(fileInfo.EndOfFile.LowPart);

	if (!ReadFile(
		file.Get(), 
		outputBuffer.data(), 
		outputBuffer.size(), 
		nullptr, 
		nullptr))
	{
		throw std::exception();
	}

	return outputBuffer;
}

uint32 Windows10IOService::WriteData(const std::wstring &filename, std::vector<byte> fileData)
{
	return 0;
}

task<std::vector<byte>> Windows10IOService::ReadDataAsync(const std::wstring &filename)
{
	auto folder = Package::Current->InstalledLocation;

	return create_task(folder->GetFileAsync(StringReference((wchar_t*)filename.c_str()))).then([=](StorageFile^ file)
	{
		return FileIO::ReadBufferAsync(file);
	}).then([](IBuffer^ fileBuffer) -> std::vector<byte>
	{
		std::vector<byte> returnBuffer;
		returnBuffer.resize(fileBuffer->Length);
		DataReader::FromBuffer(fileBuffer)->ReadBytes(ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
		return returnBuffer;
	});
}

task<uint32> Windows10IOService::WriteDataAsync(const std::wstring &filename, std::vector<byte> fileData)
{
	return task<uint32>();
}

std::wstring Windows10IOService::GetFullPath(std::wstring filename)
{
	WCHAR path[512];
	UINT pathSize = _countof(path);

	DWORD size = GetModuleFileName(nullptr, path, pathSize);
	if (size == 0 || size == pathSize)
	{
		// Method failed or path was truncated.
		throw std::exception();
	}

	WCHAR* lastSlash = wcsrchr(path, L'\\');
	if (lastSlash)
	{
		*(lastSlash + 1) = L'\0';
	}

	std::wstring pathString = path;
	pathString = pathString + filename;
	return pathString;
}

char* Windows10IOService::GetConfigFilePath()
{
	if (_configFilePath[0] == 0)
	{
		//	Get Local Folder
		String^ localfolderS = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
		//	convert folder name from wchar to ascii
		std::wstring localFolderW(localfolderS->Begin());
		std::string localFolderA(localFolderW.begin(), localFolderW.end());
		const char* localFolder = localFolderA.c_str();

		TCHAR moduleFileNameT[MAX_PATH];
		GetModuleFileName(NULL, moduleFileNameT, MAX_PATH);
		//	convert folder name from wchar to ascii
		std::wstring moduleFileNameW(moduleFileNameT);
		std::string moduleFileNameA(moduleFileNameW.begin(), moduleFileNameW.end());
		const char *moduleFileName = moduleFileNameA.c_str();

		char fileDir[MAX_PATH] = {};
		char fileName[MAX_PATH] = {};
		char fileExt[MAX_PATH] = {};
		DecomposePath(moduleFileName, fileDir, fileName, fileExt);


		snprintf(_configFilePath, MAX_PATH, "%s\\%s.config", localFolder, fileName);
	}

	return _configFilePath;
}

void Windows10IOService::DecomposePath(const char *filePath, char *fileDir, char *fileName, char *fileExt)
{
#if defined _WIN32
	const char *lastSeparator = strrchr(filePath, '\\');
#else
	const char *lastSeparator = strrchr(filePath, '/');
#endif

	const char *lastDot = strrchr(filePath, '.');
	const char *endOfPath = filePath + strlen(filePath);
	const char *startOfName = lastSeparator ? lastSeparator + 1 : filePath;
	const char *startOfExt = lastDot > startOfName ? lastDot : endOfPath;

	if (fileDir)
		_snprintf_s(fileDir, MAX_PATH, _TRUNCATE, "%.*s", startOfName - filePath, filePath);

	if (fileName)
		_snprintf_s(fileName, MAX_PATH, _TRUNCATE, "%.*s", startOfExt - startOfName, startOfName);

	if (fileExt)
		_snprintf_s(fileExt, MAX_PATH, _TRUNCATE, "%s", startOfExt);
}