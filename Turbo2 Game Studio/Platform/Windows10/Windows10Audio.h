#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboGameAudio.h>
#include <ITurboScene.h>

using namespace Microsoft::WRL;
using namespace Windows::System;

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;

namespace Turbo 
{
	namespace Platform 
	{
		namespace Windows10
		{
			// This sample renderer instantiates a basic rendering pipeline.
			class Windows10Audio : public ITurboGameAudio
			{
			public:
				Windows10Audio(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService);
				~Windows10Audio();

				//	ITurboGameAudio Methods ----------------------------------------------------------------------------
				virtual void UpdateAudioInformation();
				virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
				virtual bool PlaySounds(std::shared_ptr<ITurboScene> scene);

			private:
				std::shared_ptr<ITurboDebug>				_debug;
				std::shared_ptr<ITurboGameIOService>		_ioService;

				bool										_audioAvailable;
				ComPtr<IXAudio2>							_musicEngine;
				ComPtr<IXAudio2>							_soundEffectEngine;
				IXAudio2MasteringVoice*						_musicMasteringVoice;
				IXAudio2MasteringVoice*						_soundEffectMasteringVoice;
				IXAudio2SourceVoice*						_sourceVoice;
				WAVEFORMATEX*								_sourceFormat;

				std::map<std::string, int>					_sceneSoundEffectOffsets;
				std::map<std::string, std::vector<byte>>	_sceneSoundEffectResources;

				//	Local Support Methods --------------------------------------------------------------------------------------
				void CreateDeviceIndependentResources();
				void ReleaseSceneResources();

				void CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene);
				void LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void LoadSoundEffectData(std::string soundName, std::vector<unsigned char> *soundData);

				void PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene);
				void PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
			};
		}
	}
}
