
#pragma once

#include <pch.h>
#include <TurboGameState.h>

//  Constructors & Destructors -----------------------------------------------------------------------------------------

TurboGameState::TurboGameState()
{
}

TurboGameState::~TurboGameState()
{
}

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

std::vector<std::string> TurboGameState::Keys()
{
	std::vector<std::string> keys;
	
	for (std::map<std::string, std::string>::iterator iterator = _map.begin(); iterator != _map.end(); ++iterator) 
	{
		keys.push_back(iterator->first);
	}

	return keys;
}

//  ITurboGameState Methods --------------------------------------------------------------------------------------------
void TurboGameState::SaveBoolean(std::string key, bool value)
{

}

void TurboGameState::SaveInteger(std::string key, int value)
{

}

void TurboGameState::SaveFloat(std::string key, float value)
{

}

void TurboGameState::SaveVector(std::string key, TurboVector3D value)
{

}

void TurboGameState::SaveString(std::string key, std::string value)
{
	_map.insert(KeyValue(key, value));
}

bool TurboGameState::LoadBoolean(std::string key)
{
	return false;
}

int TurboGameState::LoadInteger(std::string key)
{
	return 0;
}

float TurboGameState::LoadFloat(std::string key)
{
	return 0.0;
}

TurboVector3D TurboGameState::LoadVector(std::string key)
{
	return TurboVector3D(0.0, 0.0, 0.0);
}

std::string TurboGameState::LoadString(std::string key)
{
	return _map[key];
}


