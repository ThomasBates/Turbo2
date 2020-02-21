
#include <pch.h>

#include <TurboGameState.h>

using namespace Turbo::Game;

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

std::vector<std::string> TurboGameState::Keys()
{
	std::vector<std::string> keys;

	for (auto& entry : _map)
		keys.push_back(entry.first);

	return keys;
}

//  ITurboGameState Methods --------------------------------------------------------------------------------------------

void TurboGameState::SaveBoolean(std::string key, bool value)
{
	_map[key] = std::to_string(value);
}

void TurboGameState::SaveInteger(std::string key, int value)
{
	_map[key] = std::to_string(value);
}

void TurboGameState::SaveFloat(std::string key, float value)
{
	_map[key] = std::to_string(value);
}

void TurboGameState::SaveVector(std::string key, TurboVector3D value)
{

}

void TurboGameState::SaveString(std::string key, std::string value)
{
	_map[key] = value;

	//_map.insert(KeyValue(key, value));
}

bool TurboGameState::LoadBoolean(std::string key, bool defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	int test = std::stoi(_map[key]);
	return test != 0;
}

int TurboGameState::LoadInteger(std::string key, int defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return std::stoi(_map[key]);
}

float TurboGameState::LoadFloat(std::string key, float defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return std::stof(_map[key]);
}

TurboVector3D TurboGameState::LoadVector(std::string key, TurboVector3D defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return TurboVector3D(0.0, 0.0, 0.0);
}

std::string TurboGameState::LoadString(std::string key, std::string defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return _map[key];
}


