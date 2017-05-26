#pragma once

#include <pch.h>

#include <ITurboGameIOService.h>

using namespace Turbo::Game;

namespace Turbo 
{
	namespace Platform
	{
		namespace Windows10
		{
			class Windows10IOService : public ITurboGameIOService
			{
			public:
				//  Constructors and Destructors -------------------------------------------------------------------------------

				//	ITurboGameIOService Methods --------------------------------------------------------------------------------
				virtual void SaveGameState(std::shared_ptr<ITurboGameState> programState);
				virtual std::shared_ptr<ITurboGameState> LoadGameState();

				virtual std::vector<byte> ReadData(const std::wstring &filename);
				virtual uint32 WriteData(const std::wstring &filename, std::vector<byte> fileData);

				virtual Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring &filename);
				virtual Concurrency::task<uint32> WriteDataAsync(const std::wstring &filename, std::vector<byte> fileData);

			private:
				std::wstring GetFullPath(LPCWSTR filename);
			};
		}	//	namespace Turbo::Platform::Windows10
	}	//	namespace Turbo::Platform
}	//	namespace Turbo
