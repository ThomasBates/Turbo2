
#include <pch.h>

#include <AndroidNDKIOService.h>
#include <TurboCoreHelpers.h>
#include <TurboGameState.h>
#include <JNIHelper.h>

using namespace Turbo::Core;
using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace ndk_helper;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKIOService::AndroidNDKIOService(std::shared_ptr<ITurboDebug> debug) :
	_debug(debug)
{
}

//	ITurboGameIOService Methods ----------------------------------------------------------------------------------------

void AndroidNDKIOService::SaveGameState(std::shared_ptr<ITurboGameState> gameState)
{
	if (gameState == nullptr)
		return;


    auto dir = JNIHelper::GetInstance()->GetExternalFilesDir() + "/" + _gameStateFileName;

	std::ofstream outFile(dir.data(), std::ios::out);

	if (!outFile)
	{
        _debug->Send(debugError, debugIOService) << "SaveGameState: Cannot write to file: " << dir << "\n";
        return;
    }

    _debug->Send(debugDebug, debugIOService) << "SaveGameState: Writing to file: " << dir << "\n";

    std::vector<std::string> keys = gameState->Keys();
    for (const auto& key : keys)
    {
        std::string value = gameState->LoadString(key, "");
        _debug->Send(debugDebug, debugIOService) << "SaveGameState: " << key << " = " << value << "\n";

        value = SanitizeStringValue(value);
        outFile << key << "=" << value << '\n';
    }

    outFile.close();
}

std::shared_ptr<ITurboGameState> AndroidNDKIOService::LoadGameState()
{
	std::shared_ptr<ITurboGameState> gameState = std::shared_ptr<ITurboGameState>(new TurboGameState());

    auto dir = JNIHelper::GetInstance()->GetExternalFilesDir() + "/" + _gameStateFileName;

    std::ifstream inFile(dir.data(), std::ios::in);

    if (!inFile)
    {
        _debug->Send(debugError, debugIOService) << "LoadGameState: Cannot read from file: " << dir << "\n";
        return gameState;
    }

    _debug->Send(debugDebug, debugIOService) << "LoadGameState: Reading from file: " << dir << "\n";

    std::string line;

    while (inFile >> line)
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        auto key = line.substr(0, pos);
        auto value = line.substr(pos+1);
        value = RestoreStringValue(value);

        _debug->Send(debugDebug, debugIOService) << "LoadGameState: " << key << " = " << value << "\n";
        gameState->SaveString(key, value);
    }

    inFile.close();

	return gameState;
}

std::wstring AndroidNDKIOService::GetFullPath(std::wstring filename)
{
    return std::wstring();
}
std::vector<unsigned char> AndroidNDKIOService::ReadData(const std::wstring &filename)
{
	std::vector<unsigned char> data; //  = std::vector<unsigned char>();

	std::string temp = ToString(filename);
	const char *fileName = temp.data();

	if (!JNIHelper::GetInstance()->ReadFile(fileName, &data))
	{
		_debug->Send(debugError, debugIOService) << "Can not open a file: " << fileName << "\n";
	}

	return data;
}

int AndroidNDKIOService::WriteData(const std::wstring &filename, std::vector<unsigned char> fileData)
{
	return 0;
}

//  Private Methods ----------------------------------------------------------------------------------------------------

std::string AndroidNDKIOService::SanitizeStringValue(std::string value)
{
    auto result = move(value);

    ReplaceAll(result, " ", "%sp;");
    ReplaceAll(result, "\n", "%nl;");
    ReplaceAll(result, "\r", "%cr;");
    ReplaceAll(result, "\t", "%tab;");

    return result;
}

std::string AndroidNDKIOService::RestoreStringValue(std::string value)
{
    auto result = move(value);

    ReplaceAll(result, "%sp;", " ");
    ReplaceAll(result, "%nl;", "\n");
    ReplaceAll(result, "%cr;", "\r");
    ReplaceAll(result, "%tab;", "\t");

    return result;
}

void AndroidNDKIOService::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;

    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}