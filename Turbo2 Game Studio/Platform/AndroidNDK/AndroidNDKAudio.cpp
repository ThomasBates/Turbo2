
//#include <windows.h>
#include <pch.h>
#include <iostream>
#include <string>

#include <ITurboImage.h>
#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>

#include <AndroidNDKAudio.h>
//#include <AndroidNDKHelpers.h>

//using namespace Microsoft::WRL;
//using namespace Windows::Foundation;
//using namespace Windows::Graphics::Display;
//using namespace Windows::UI::Core;

using namespace Turbo::Core::Debug;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::AndroidNDK;
using namespace Turbo::Platform::AndroidNDK;

//	https://docs.microsoft.com/en-us/windows/uwp/gaming/working-with-audio-in-your-directx-game

#pragma region Constructors and Destructors ----------------------------------------------------------------------------

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
AndroidNDKAudio::AndroidNDKAudio(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboGameIOService> ioService) :
	_debug(debug),
	_ioService(ioService)
{
	UpdateAudioInformation();
}

AndroidNDKAudio::~AndroidNDKAudio()
{
	ReleaseSceneResources();
}

#pragma endregion
#pragma region Methods -------------------------------------------------------------------------------------------------

void AndroidNDKAudio::UpdateAudioInformation()
{
	CreateDeviceDependentResources();
}

bool AndroidNDKAudio::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return false;
	}

	ReleaseSceneResources();

	CreateSceneSoundEffectResources(scene);

	return true;
}

// Renders one frame using the vertex and pixel shaders.
bool AndroidNDKAudio::PlaySounds(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return false;
	}

	PlaySceneSoundEffects(scene);

	return true;
}

#pragma endregion
#pragma region Local Support Methods -----------------------------------------------------------------------------------

void AndroidNDKAudio::CreateDeviceDependentResources()
{
    // not yet implemented.
	_audioAvailable = false;
}

void AndroidNDKAudio::ReleaseSceneResources()
{
	//_sceneSoundEffectFormat.clear();
	//_sceneSoundEffectBuffer.clear();
}

//	Scene Sound Effect Resources ---------------------------------------------------------------------------------------

void AndroidNDKAudio::CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene)
{
	if (scene == nullptr)
	{
		return;
	}

	auto sceneObjects = scene->SceneObjects();

	for (auto& sceneObject : sceneObjects)
	{
		LoadSceneObjectTreeSoundEffects(sceneObject);
	}
}

void AndroidNDKAudio::LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	LoadSceneObjectSoundEffects(sceneObject);

	//  Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		LoadSceneObjectTreeSoundEffects(childSceneObject);
	}
}

void AndroidNDKAudio::LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	std::shared_ptr<ITurboSceneSoundEffect> hitSound = sceneObject->HitSound();

	if (hitSound == nullptr)
	{
		return;
	}

	std::string soundName = hitSound->Name();

	LoadSoundEffectData(soundName);
}

bool AndroidNDKAudio::LoadSoundEffectData(std::string soundName)
{
	//  Already loaded this texture? don't reload it.
//	if (_sceneSoundEffectFormat.find(soundName) != _sceneSoundEffectFormat.end())
//	{
//		return true;
//	}

	//  not yet implemented
	return false;
}

//	Rendering Support Methods ------------------------------------------------------------------------------------------

void AndroidNDKAudio::PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene)
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

void AndroidNDKAudio::PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	PlaySceneObjectSoundEffects(sceneObject);

	//	Iterate over child scene objects. Call this method recursively.
	for (auto& childSceneObject : sceneObject->ChildSceneObjects())
	{
		PlaySceneObjectTreeSoundEffects(childSceneObject);
	}
}

void AndroidNDKAudio::PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject)
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

	sound->PlaySound(false);

	if (sound->Volume() <= 0.0)
	{
		return;
	}

	if (!_audioAvailable)
	{
		// Audio is not available so just return.
		return;
	}
	
	std::string soundName = sound->Name();

	LoadSoundEffectData(soundName);

	//	sound name not found?
//	if (_sceneSoundEffectFormat.find(soundName) == _sceneSoundEffectFormat.end())
//	{
//		return;
//	}

	//  Not yet implemented
}

#pragma endregion

