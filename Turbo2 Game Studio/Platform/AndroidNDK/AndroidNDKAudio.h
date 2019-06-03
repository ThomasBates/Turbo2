#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboGameAudio.h>
#include <ITurboScene.h>

//using namespace Microsoft::WRL;
//using namespace Windows::System;

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;


namespace Turbo 
{
	namespace Platform 
	{
		namespace AndroidNDK
		{
			// This sample renderer instantiates a basic rendering pipeline.
			class AndroidNDKAudio : public ITurboGameAudio
			{
			public:
				AndroidNDKAudio(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService);
				virtual ~AndroidNDKAudio();

				//	ITurboGameAudio Methods ----------------------------------------------------------------------------
				virtual void UpdateAudioInformation();
				virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
				virtual void ReleaseSceneResources();
				virtual bool PlaySounds(std::shared_ptr<ITurboScene> scene);

			private:
				std::shared_ptr<ITurboDebug>				_debug;
				std::shared_ptr<ITurboGameIOService>		_ioService;

				bool										_audioAvailable;
//				ComPtr<IAndroidNDK>							_AndroidNDK;
//				IAndroidNDKMasteringVoice*					_pMasterVoice = NULL;
//
//				std::map<std::string, std::vector<byte>>	_sceneSoundEffectResources;
//
//				std::map<std::string, WAVEFORMATEXTENSIBLE>	_sceneSoundEffectFormat;
//				std::map<std::string, AndroidNDK_BUFFER>	_sceneSoundEffectBuffer;

				//	Local Support Methods --------------------------------------------------------------------------------------
				void CreateDeviceDependentResources();

				void CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene);
				void LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				bool LoadSoundEffectData(std::string soundName);

				void PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene);
				void PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
			};
		}
	}
}
