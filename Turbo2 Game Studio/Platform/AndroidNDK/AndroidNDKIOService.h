#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameIOService.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

#define MAX_PATH 1024

namespace Turbo 
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			class AndroidNDKIOService : public ITurboGameIOService
			{
			public:
				//  Constructors and Destructors -----------------------------------------------------------------------
				AndroidNDKIOService(std::shared_ptr<ITurboDebug> debug);
				virtual ~AndroidNDKIOService() {};

				//	ITurboGameIOService Methods ------------------------------------------------------------------------
				virtual void SaveGameState(std::shared_ptr<ITurboGameState> gameState);
				virtual std::shared_ptr<ITurboGameState> LoadGameState();

				virtual std::wstring GetFullPath(std::wstring filename);
				virtual std::vector<unsigned char> ReadData(const std::wstring &filename);
				virtual int WriteData(const std::wstring &filename, std::vector<unsigned char> fileData);

			private:
				std::shared_ptr<ITurboDebug> _debug;

                const std::string _gameStateFileName = "GameState.txt";

                std::string SanitizeStringValue(std::string value);
                std::string RestoreStringValue(std::string value);
                void ReplaceAll(std::string &str, const std::string &from, const std::string &to);
            };
		}
	}
}
