//  ========================================================================  //
//  DSBitmap.h
//  ========================================================================  //

#pragma once

#include <pch.h>

#include <ITurboSoundFormat.h>
#include <ITurboSoundCanvas.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Debug;

namespace Turbo
{
    namespace Sound
    {
        typedef struct
        {
            char chunkID[4];
            uint32_t chunkSize;
        } ChunkHeader;

        typedef struct
        {
            ChunkHeader header;
            char waveID[4];
        } RIFFChunk;

        typedef struct
        {
            ChunkHeader header;
            uint16_t format;
            uint16_t channels;
            uint32_t sampleRate;
            uint32_t dataRate;
            uint16_t blockSize;
            uint16_t sampleSize;
        } FormatChunk;

        typedef struct
        {
            ChunkHeader header;
            unsigned char dataStart;
        } DataChunk;

        typedef struct
        {
            RIFFChunk riffChunk;
            FormatChunk fmtChunk;
            DataChunk dataChunk;
        } WavFile;

        class TurboWavFormat : public ITurboSoundFormat
        {
        public:
            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboWavFormat(std::shared_ptr<ITurboDebug> debug, ITurboSoundCanvas *canvas = 0, void *data = 0);
            virtual ~TurboWavFormat() {}

            //  ITurboSoundFormat Properties ---------------------------------------------------------------------------
            virtual ITurboSoundCanvas* Canvas() { return _canvas; }
            virtual void Canvas(ITurboSoundCanvas *canvas) { _canvas = canvas; }

            virtual void* Data() { return (void*)_data; }
            virtual void Data(void* data);

            virtual int SampleRate() { return _sampleRate; }
            virtual void SampleRate(int sampleRate) { _sampleRate = sampleRate; }

            virtual int ChannelCount() { return _channelCount; }
            virtual void ChannelCount(int channelCount) { _channelCount = channelCount; }

            virtual int Length() { return _length; }
            virtual void Length(int length) { _length = length; }

            //  ITurboSoundFormat Methods ------------------------------------------------------------------------------
            virtual void RenderToCanvas(int sampleRate, int channelCount, int length);

        private:
            std::shared_ptr<ITurboDebug> _debug;
            ITurboSoundCanvas* _canvas;
            unsigned char* _data;

            int _sampleRate {0};
            int _channelCount {0};
            int _length {0};

            WavFile* _wavFile;

            void RenderSimple();
            void RenderScale();
        };
    }
}
