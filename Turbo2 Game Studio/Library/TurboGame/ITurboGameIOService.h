
#pragma once

#include <ITurboGameState.h>

namespace Turbo
{
	namespace Game
	{
		class ITurboGameIOService
		{
		public:
			//	ITurboGameIOService Methods --------------------------------------------------------------------------------
			virtual void SaveGameState(std::shared_ptr<ITurboGameState> programState) = 0;
			virtual std::shared_ptr<ITurboGameState> LoadGameState() = 0;

			virtual std::wstring GetFullPath(std::wstring filename) = 0;
			virtual std::vector<byte> ReadData(const std::wstring &filename) = 0;
			virtual uint32 WriteData(const std::wstring &filename, std::vector<byte> fileData) = 0;

			virtual Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring &filename) = 0;
			virtual Concurrency::task<uint32> WriteDataAsync(const std::wstring &filename, std::vector<byte> fileData) = 0;
		};
	}
}
