
#pragma once

#include <pch.h>
#include <IApplicationState.h>

typedef std::pair<std::string, std::string> KeyValue;

class ApplicationState : public IApplicationState
{
public:
	//  Constructors & Destructors  ------------------------------------------------------------------------------------
	ApplicationState();
	~ApplicationState();

	//  IApplicationState Properties  ----------------------------------------------------------------------------------
	virtual std::vector<std::string> Keys();

	//  IApplicationState Methods  -------------------------------------------------------------------------------------
	virtual void SaveBoolean(std::string key, bool value);
	virtual void SaveInteger(std::string key, int value);
	virtual void SaveFloat(std::string key, float value);
	virtual void SaveVector(std::string key, Vector3D value);
	virtual void SaveString(std::string key, std::string value);

	virtual bool LoadBoolean(std::string key);
	virtual int LoadInteger(std::string key);
	virtual float LoadFloat(std::string key);
	virtual Vector3D LoadVector(std::string key);
	virtual std::string LoadString(std::string key);

private:
	std::map<std::string, std::string> _map;

};