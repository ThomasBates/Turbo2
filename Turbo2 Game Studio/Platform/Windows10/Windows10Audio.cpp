
#include <pch.h>

#include <ITurboImage.h>
#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>

#include <Windows10Audio.h>
#include <Windows10Helpers.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::Windows10;

//	https://docs.microsoft.com/en-us/windows/uwp/gaming/working-with-audio-in-your-directx-game

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Windows10Audio::Windows10Audio(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService) :
	_debug(debug),
	_ioService(ioService)
{
}

Windows10Audio::~Windows10Audio()
{
	ReleaseSceneResources();
}

#pragma endregion
#pragma region Methods -------------------------------------------------------------------------------------------------

void Windows10Audio::UpdateAudioInformation()
{
	CreateDeviceIndependentResources();
}

bool Windows10Audio::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
	ReleaseSceneResources();

	CreateSceneSoundEffectResources(scene);

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool Windows10Audio::PlaySounds(std::shared_ptr<ITurboScene> scene)
{
	PlaySceneSoundEffects(scene);

	return true;
}

#pragma endregion
#pragma region Local Support Methods -----------------------------------------------------------------------------------

void Windows10Audio::CreateDeviceIndependentResources()
{
	UINT32 flags = 0;

	ThrowIfFailed(
		XAudio2Create(&_musicEngine, flags)
		);

#if defined(_DEBUG)
	XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
	debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
	debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
	_musicEngine->SetDebugConfiguration(&debugConfiguration);
#endif


	HRESULT hr = _musicEngine->CreateMasteringVoice(&_musicMasteringVoice);
	if (FAILED(hr))
	{
		// Unable to create an audio device
		_audioAvailable = false;
		return;
	}

	ThrowIfFailed(
		XAudio2Create(&_soundEffectEngine, flags)
		);

#if defined(_DEBUG)
	_soundEffectEngine->SetDebugConfiguration(&debugConfiguration);
#endif

	ThrowIfFailed(
		_soundEffectEngine->CreateMasteringVoice(&_soundEffectMasteringVoice)
		);

	ThrowIfFailed(_soundEffectEngine->CreateSourceVoice(&_sourceVoice, _sourceFormat));


	_audioAvailable = true;
}

void Windows10Audio::ReleaseSceneResources()
{

}

//	Scene Sound Effect Resources ---------------------------------------------------------------------------------------

void Windows10Audio::CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene)
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

void Windows10Audio::LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	LoadSceneObjectSoundEffects(sceneObject);

	//  Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTreeSoundEffects(childSceneObject);
	}
}

void Windows10Audio::LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
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

	LoadSoundEffectData(soundName, &soundData);
	
	_sceneSoundEffectResources[soundName] = soundData;
}

void Windows10Audio::LoadSoundEffectData(std::string soundName, std::vector<unsigned char>* soundData)
{
	//std::vector<byte> fileData = _ioService->ReadData(ToWString(soundName + ".wav"));
}

//	Rendering Support Methods ------------------------------------------------------------------------------------------

void Windows10Audio::PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene)
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

void Windows10Audio::PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	PlaySceneObjectSoundEffects(sceneObject);

	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		PlaySceneObjectTreeSoundEffects(childSceneObject);
	}
}

void Windows10Audio::PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
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

	XAUDIO2_BUFFER buffer = { 0 };

	if (!_audioAvailable)
	{
		// Audio is not available so just return.
		return;
	}

	// Interrupt sound effect if it is currently playing.
	ThrowIfFailed(
		_sourceVoice->Stop()
		);
	ThrowIfFailed(
		_sourceVoice->FlushSourceBuffers()
		);

	// Queue the memory buffer for playback and start the voice.
	//buffer.AudioBytes = _soundData->Length;
	//buffer.pAudioData = _soundData->Data;
	//buffer.Flags = XAUDIO2_END_OF_STREAM;

	//ThrowIfFailed(
	//	_sourceVoice->SetVolume(volume)
	//	);
	//ThrowIfFailed(
	//	_sourceVoice->SubmitSourceBuffer(&buffer)
	//	);
	//ThrowIfFailed(
	//	_sourceVoice->Start()
	//	);


}

#pragma endregion

