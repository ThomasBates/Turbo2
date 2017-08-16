#include <pch.h>

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

void Windows10IOService::SaveGameState(std::shared_ptr<ITurboGameState> programState)
{
}

std::shared_ptr<ITurboGameState> Windows10IOService::LoadGameState()
{
	return std::shared_ptr<ITurboGameState>();
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
