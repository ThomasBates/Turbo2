
#include <pch.h>

#include <utility>

#include <AndroidNDKConfigProvider.h>
#include <TurboCoreHelpers.h>
#include <TurboGameState.h>
#include <JNIHelper.h>

using namespace Turbo::Core;
using namespace Turbo::Core::Debug;
using namespace Turbo::Platform::AndroidNDK;
using namespace ndk_helper;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKConfigProvider::AndroidNDKConfigProvider(std::shared_ptr<ITurboDebug> debug, std::string name) :
	_debug(std::move(debug)),
	_name(std::move(name))
{
    if (_name.empty())
    {
        _name = "GameState.txt";
    }

    _location = JNIHelper::GetInstance()->GetExternalFilesDir() + "/" + _name;
    _debug->Send(debugDebug, debugConfig) << "AndroidNDKConfigProvider: _location = '" << _location << "'\n";
}

//	ITurboGameConfigProvider Properties -------------------------------------------------------------------------------------

bool AndroidNDKConfigProvider::IsInitialized()
{
    if (_isDocumentLoaded)
    {
        return true;
    }

    auto location = JNIHelper::GetInstance()->GetExternalFilesDir();
    if (location.empty())
    {
        return false;
    }

    _location = location + "/" + _name;
    _debug->Send(debugDebug, debugConfig) << "IsInitialized: _location = '" << _location << "'\n";

    LoadDocument();

    return _isDocumentLoaded;
}

//	ITurboGameConfigProvider Methods ----------------------------------------------------------------------------------------

void AndroidNDKConfigProvider::WriteString(std::string path, std::string value, bool isDefaultValue)
{
    LoadDocument();
    _dictionary[path] = value;
    SaveDocument();
}

std::string AndroidNDKConfigProvider::ReadString(std::string path)
{
    LoadDocument();

    if (_dictionary.find(path) == _dictionary.end())
        return "";

    return _dictionary[path];
}

void AndroidNDKConfigProvider::BeginBatch()
{
    if (_batchCount < 0)
    {
        // throw new IndexOutOfRangeException("Mismatched BeginBatch/EndBatch");
    }

//    if (_batchCount == 0)
//    {
//        _isDocumentLoaded = false;
//        LoadDocument();
//    }

    _batchCount++;
}

void AndroidNDKConfigProvider::EndBatch()
{
    _batchCount--;

    if (_batchCount < 0)
    {
        // throw new IndexOutOfRangeException("Mismatched BeginBatch/EndBatch");
    }

    if ((_batchCount == 0) && _isDirty)
    {
        SaveDocument();
    }
}

//  Private Methods ----------------------------------------------------------------------------------------------------

void AndroidNDKConfigProvider::LoadDocument()
{
    if (IsInBatch())
    {
        return;
    }

    if (_isDocumentLoaded)
    {
        return;
    }

    std::ifstream inFile(_location.data(), std::ios::in);

    if (!inFile)
    {
        _debug->Send(debugError, debugConfig) << "LoadDocument: Cannot read from file: " << _location << "\n";
        return;
    }

    _dictionary.clear();

    _debug->Send(debugDebug, debugConfig) << "LoadDocument: Reading from file: " << _location << "\n";

    std::string line;

    while (inFile >> line)
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        auto key = line.substr(0, pos);
        auto value = line.substr(pos+1);
        value = RestoreStringValue(value);

        _debug->Send(debugDebug, debugConfig) << "LoadDocument: " << key << " = " << value << "\n";

        _dictionary[key] = value;
    }

    inFile.close();

    _isDocumentLoaded = true;
}

void AndroidNDKConfigProvider::SaveDocument()
{
    if (IsInBatch())
    {
        _isDirty = true;
        return;
    }

    std::ofstream outFile(_location.data(), std::ios::out);

    if (!outFile)
    {
        _debug->Send(debugError, debugConfig) << "SaveDocument: Cannot write to file: " << _location << "\n";
        return;
    }

    _debug->Send(debugDebug, debugConfig) << "SaveDocument: Writing to file: " << _location << "\n";

    for (const auto& entry : _dictionary)
    {
        auto key = entry.first;
        auto value = entry.second;

        _debug->Send(debugDebug, debugConfig) << "SaveDocument: " << key << " = " << value << "\n";

        value = SanitizeStringValue(value);
        outFile << key << "=" << value << '\n';
    }

    outFile.close();

    _isDocumentLoaded = true;
}

std::string AndroidNDKConfigProvider::SanitizeStringValue(std::string value)
{
    auto result = move(value);

    ReplaceAll(result, " ", "%sp;");
    ReplaceAll(result, "\n", "%nl;");
    ReplaceAll(result, "\r", "%cr;");
    ReplaceAll(result, "\t", "%tab;");

    return result;
}

std::string AndroidNDKConfigProvider::RestoreStringValue(std::string value)
{
    auto result = move(value);

    ReplaceAll(result, "%sp;", " ");
    ReplaceAll(result, "%nl;", "\n");
    ReplaceAll(result, "%cr;", "\r");
    ReplaceAll(result, "%tab;", "\t");

    return result;
}

void AndroidNDKConfigProvider::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
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