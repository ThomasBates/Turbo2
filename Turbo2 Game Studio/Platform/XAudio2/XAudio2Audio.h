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


#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

namespace Turbo 
{
	namespace Platform 
	{
		namespace XAudio2
		{
			// This sample renderer instantiates a basic rendering pipeline.
			class XAudio2Audio : public ITurboGameAudio
			{
			public:
				XAudio2Audio(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboGameIOService> ioService);
				~XAudio2Audio();

				//	ITurboGameAudio Methods ----------------------------------------------------------------------------
				virtual void UpdateAudioInformation();
				virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
				virtual bool PlaySounds(std::shared_ptr<ITurboScene> scene);

			private:
				std::shared_ptr<ITurboDebug>				_debug;
				std::shared_ptr<ITurboGameIOService>		_ioService;

				bool										_audioAvailable;
				ComPtr<IXAudio2>							_XAudio2;
				IXAudio2MasteringVoice*						_pMasterVoice = NULL;

				std::map<std::string, std::vector<unsigned char>>	_sceneSoundEffectResources;

				std::map<std::string, WAVEFORMATEXTENSIBLE>	_sceneSoundEffectFormat;
				std::map<std::string, XAUDIO2_BUFFER>		_sceneSoundEffectBuffer;

				//	Local Support Methods --------------------------------------------------------------------------------------
				void CreateDeviceDependentResources();
				void ReleaseSceneResources();

				void CreateSceneSoundEffectResources(std::shared_ptr<ITurboScene> scene);
				void LoadSceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void LoadSceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				bool LoadSoundEffectData(std::string soundName);

				void PlaySceneSoundEffects(std::shared_ptr<ITurboScene> scene);
				void PlaySceneObjectTreeSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);
				void PlaySceneObjectSoundEffects(std::shared_ptr<ITurboSceneObject> sceneObject);

				HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
				HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
			};
		}
	}
}
