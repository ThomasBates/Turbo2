
#include <pch.h>
#include <utility>

#include <TurboConfigMemoryProvider.h>

using namespace Turbo::Core::Config;

TurboConfigMemoryProvider::TurboConfigMemoryProvider(std::string name) :
	_name(std::move(name))
{
	if (_name.empty())
	{
		_name = "Memory";
	}
}

void TurboConfigMemoryProvider::WriteString(std::string path, std::string value, bool isDefaultValue)
{
	_dictionary[path] = value;
}

std::string TurboConfigMemoryProvider::ReadString(std::string path)
{
	if (_dictionary.find(path) == _dictionary.end())
			return "";

	return _dictionary[path];
}
