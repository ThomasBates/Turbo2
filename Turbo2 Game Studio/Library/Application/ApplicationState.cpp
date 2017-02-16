
#pragma once

#include <pch.h>
#include <ApplicationState.h>

//  Constructors & Destructors  ------------------------------------------------------------------------------------

ApplicationState::ApplicationState()
{
}

ApplicationState::~ApplicationState()
{
}

//  IApplicationState Properties  ----------------------------------------------------------------------------------

std::vector<std::string> ApplicationState::Keys()
{
	std::vector<std::string> keys;
	
	for (std::map<std::string, std::string>::iterator iterator = _map.begin(); iterator != _map.end(); ++iterator) 
	{
		keys.push_back(iterator->first);
	}

	return keys;
}

//  IApplicationState Methods  -------------------------------------------------------------------------------------
void ApplicationState::SaveBoolean(std::string key, bool value)
{

}

void ApplicationState::SaveInteger(std::string key, int value)
{

}

void ApplicationState::SaveFloat(std::string key, float value)
{

}

void ApplicationState::SaveVector(std::string key, Vector3D value)
{

}

void ApplicationState::SaveString(std::string key, std::string value)
{
	_map.insert(KeyValue(key, value));
}

bool ApplicationState::LoadBoolean(std::string key)
{
	return false;
}

int ApplicationState::LoadInteger(std::string key)
{
	return 0;
}

float ApplicationState::LoadFloat(std::string key)
{
	return 0.0;
}

Vector3D ApplicationState::LoadVector(std::string key)
{
	return Vector3D(0.0, 0.0, 0.0);
}

std::string ApplicationState::LoadString(std::string key)
{
	return _map[key];
}


