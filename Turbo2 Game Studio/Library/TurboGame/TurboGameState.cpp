
#include <pch.h>

#include <TurboGameState.h>

//  Constructors & Destructors -----------------------------------------------------------------------------------------

Turbo::Game::TurboGameState::TurboGameState()
{
}

Turbo::Game::TurboGameState::~TurboGameState()
{
}

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

std::vector<std::string> Turbo::Game::TurboGameState::Keys()
{
	std::vector<std::string> keys;
	
	for (std::map<std::string, std::string>::iterator iterator = _map.begin(); iterator != _map.end(); ++iterator) 
	{
		keys.push_back(iterator->first);
	}

	return keys;
}

//  ITurboGameState Methods --------------------------------------------------------------------------------------------
void Turbo::Game::TurboGameState::SaveBoolean(std::string key, bool value)
{
	_map[key] = std::to_string(value);
}

void Turbo::Game::TurboGameState::SaveInteger(std::string key, int value)
{
	_map[key] = std::to_string(value);
}

void Turbo::Game::TurboGameState::SaveFloat(std::string key, float value)
{
	_map[key] = std::to_string(value);
}

void Turbo::Game::TurboGameState::SaveVector(std::string key, TurboVector3D value)
{

}

void Turbo::Game::TurboGameState::SaveString(std::string key, std::string value)
{
	_map[key] = value;

	//_map.insert(KeyValue(key, value));
}

bool Turbo::Game::TurboGameState::LoadBoolean(std::string key, bool defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	int test = std::stoi(_map[key]);
	return test != 0;
}

int Turbo::Game::TurboGameState::LoadInteger(std::string key, int defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return std::stoi(_map[key]);
}

float Turbo::Game::TurboGameState::LoadFloat(std::string key, float defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return std::stof(_map[key]);
}

TurboVector3D Turbo::Game::TurboGameState::LoadVector(std::string key, TurboVector3D defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return TurboVector3D(0.0, 0.0, 0.0);
}

std::string Turbo::Game::TurboGameState::LoadString(std::string key, std::string defaultValue)
{
	//	key not in map?
	if (_map.find(key) == _map.end())
	{
		return defaultValue;
	}

	return _map[key];
}


