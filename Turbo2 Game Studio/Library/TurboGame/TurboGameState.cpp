
#pragma once

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

}

void Turbo::Game::TurboGameState::SaveInteger(std::string key, int value)
{

}

void Turbo::Game::TurboGameState::SaveFloat(std::string key, float value)
{

}

void Turbo::Game::TurboGameState::SaveVector(std::string key, TurboVector3D value)
{

}

void Turbo::Game::TurboGameState::SaveString(std::string key, std::string value)
{
	_map.insert(KeyValue(key, value));
}

bool Turbo::Game::TurboGameState::LoadBoolean(std::string key)
{
	return false;
}

int Turbo::Game::TurboGameState::LoadInteger(std::string key)
{
	return 0;
}

float Turbo::Game::TurboGameState::LoadFloat(std::string key)
{
	return 0.0;
}

TurboVector3D Turbo::Game::TurboGameState::LoadVector(std::string key)
{
	return TurboVector3D(0.0, 0.0, 0.0);
}

std::string Turbo::Game::TurboGameState::LoadString(std::string key)
{
	return _map[key];
}


