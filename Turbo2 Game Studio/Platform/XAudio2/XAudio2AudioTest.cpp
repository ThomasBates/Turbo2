
#include <windows.h>
#include <pch.h>
#include <iostream>
#include <string>

#include <ITurboImage.h>
#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>

#include <XAudio2AudioTest.h>
#include <Windows10Helpers.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::XAudio2;
using namespace Turbo::Platform::Windows10;

//	https://docs.microsoft.com/en-us/windows/uwp/gaming/working-with-audio-in-your-directx-game

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
XAudio2AudioTest::XAudio2AudioTest(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService) :
	_debug(debug),
	_ioService(ioService)
{
	UpdateAudioInformation();
}

XAudio2AudioTest::~XAudio2AudioTest()
{
	ReleaseSceneResources();
}

#pragma endregion
#pragma region Methods -------------------------------------------------------------------------------------------------

void XAudio2AudioTest::UpdateAudioInformation()
{
	CreateDeviceIndependentResources();
}

bool XAudio2AudioTest::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
	ReleaseSceneResources();

	CreateSceneSoundEffectResources(scene);

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool XAudio2AudioTest::PlaySounds(std::shared_ptr<ITurboScene> scene)
{
	PlaySceneSoundEffects(scene);

	return true;
}

#pragma endregion
#pragma region Local Support Methods -----------------------------------------------------------------------------------

void XAudio2AudioTest::CreateDeviceIndependentResources()
{
	ThrowIfFailed(XAudio2Create(&_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));

	ThrowIfFailed(_pXAudio2->CreateMasteringVoice(&_pMasterVoice));

	_audioAvailable = true;
}

void XAudio2AudioTest::ReleaseSceneResources()
{

}

//	Scene Sound Effect Resources ---------------------------------------------------------------------------------------

void XAudio2AudioTest::CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	//	Prepare data structures.
	_sceneSoundEffectResources.clear();

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		LoadSceneObjectTreeSoundEffects(sceneObject);
	}
}

void XAudio2AudioTest::LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	LoadSceneObjectSoundEffects(sceneObject);

	//  Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTreeSoundEffects(childSceneObject);
	}
}

void XAudio2AudioTest::LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	std::shared_ptr<ITurboSceneSoundEffect> hitSound = sceneObject->HitSound();

	if (hitSound == nullptr)
	{
		return;
	}

	std::string soundName = hitSound->Name();

	//  Already loaded this texture? don't reload it.
	if (_sceneSoundEffectOffsets.find(soundName) != _sceneSoundEffectOffsets.end())
	{
		return;
	}

	int soundEffectOffset = _sceneSoundEffectOffsets.size();
	_sceneSoundEffectOffsets[soundName] = soundEffectOffset;

	std::vector<unsigned char> soundData;

	_sceneSoundEffectFormat[soundName] = WAVEFORMATEXTENSIBLE();
	_sceneSoundEffectBuffer[soundName] = XAUDIO2_BUFFER();

	WAVEFORMATEXTENSIBLE* pFormat = &_sceneSoundEffectFormat[soundName];
	XAUDIO2_BUFFER*       pBuffer = &_sceneSoundEffectBuffer[soundName];

	LoadSoundEffectData(soundName, pFormat, pBuffer);
}

bool XAudio2AudioTest::LoadSoundEffectData(std::string soundName, WAVEFORMATEXTENSIBLE* pFormat, XAUDIO2_BUFFER* pBuffer)
{

	std::wstring fileName = _ioService->GetFullPath(ToWString(soundName + ".wav"));

	//	2.	Open the audio file with CreateFile. 
	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	// Open the file
	HANDLE hFile = CreateFile2(
		fileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING,
		&extendedParams);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return (HRESULT_FROM_WIN32(GetLastError()) == S_OK);
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		return (HRESULT_FROM_WIN32(GetLastError()) == S_OK);
	}

	//	3.	Locate the 'RIFF' chunk in the audio file, and check the file type. 
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
	{
		return false;
	}

	//	4.	Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure. 
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, pFormat, dwChunkSize, dwChunkPosition);

	//	5.	Locate the 'data' chunk, and read its contents into a buffer. 
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//	6.	Populate an XAUDIO2_BUFFER structure. 
	pBuffer->AudioBytes = dwChunkSize;  //buffer containing audio data
	pBuffer->pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	pBuffer->Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	CloseHandle(hFile);

	return true;
}

//	Rendering Support Methods ------------------------------------------------------------------------------------------

void XAudio2AudioTest::PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		PlaySceneObjectTreeSoundEffects(sceneObject);
	}
}

void XAudio2AudioTest::PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	PlaySceneObjectSoundEffects(sceneObject);

	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		PlaySceneObjectTreeSoundEffects(childSceneObject);
	}
}

void XAudio2AudioTest::PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	std::shared_ptr<ITurboSceneSoundEffect> sound = sceneObject->HitSound();
	if (sound == nullptr)
	{
		return;
	}

	if (!sound->PlaySound())
	{
		return;
	}

	if (sound->Volume() <= 0.0)
	{
		return;
	}

	sound->PlaySound(false);

	if (!_audioAvailable)
	{
		// Audio is not available so just return.
		return;
	}

	std::string soundName = sound->Name();

	//	sound name not found?
	if (_sceneSoundEffectOffsets.find(soundName) == _sceneSoundEffectOffsets.end())
	{
		return;
	}

	PlaySoundWithXAudio2(soundName);
}

HRESULT XAudio2AudioTest::PlaySoundWithXAudio2(std::string soundName)
{
	WAVEFORMATEXTENSIBLE* pFormat = &_sceneSoundEffectFormat[soundName];
	XAUDIO2_BUFFER*       pBuffer = &_sceneSoundEffectBuffer[soundName];

	//	Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine. 
	//	The format of the voice is specified by the values set in a WAVEFORMATEX structure. 

	ThrowIfFailed(_pXAudio2->CreateSourceVoice(&_pSourceVoice, (WAVEFORMATEX*)pFormat));

	//	Submit an XAUDIO2_BUFFER to the source voice using the function SubmitSourceBuffer. 
	ThrowIfFailed(_pSourceVoice->SubmitSourceBuffer(pBuffer));
	
	ThrowIfFailed(_pSourceVoice->Start(0));

	return 0;
}

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT  'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

HRESULT XAudio2AudioTest::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT XAudio2AudioTest::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

#pragma endregion
