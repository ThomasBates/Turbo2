
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
			virtual void SaveGameState(std::shared_ptr<ITurboGameState> gameState) = 0;
			virtual std::shared_ptr<ITurboGameState> LoadGameState() = 0;

			virtual std::wstring GetFullPath(std::wstring filename) = 0;
			virtual std::vector<unsigned char> ReadData(const std::wstring &filename) = 0;
			virtual int WriteData(const std::wstring &filename, std::vector<unsigned char> fileData) = 0;

			//virtual Concurrency::task<std::vector<unsigned char>> ReadDataAsync(const std::wstring &filename) = 0;
			//virtual Concurrency::task<uint32> WriteDataAsync(const std::wstring &filename, std::vector<unsigned char> fileData) = 0;
		};
	}
}
