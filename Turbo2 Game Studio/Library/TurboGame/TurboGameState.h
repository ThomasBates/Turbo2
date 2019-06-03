
#pragma once

#include <pch.h>
#include <ITurboGameState.h>

typedef std::pair<std::string, std::string> KeyValue;

namespace Turbo
{
	namespace Game
	{
		class TurboGameState : public ITurboGameState
		{
		public:
			//  Constructors & Destructors -------------------------------------------------------------------------------------
			TurboGameState();
			virtual ~TurboGameState() {}

			//  ITurboGameState Properties -------------------------------------------------------------------------------------
			virtual std::vector<std::string> Keys();

			//  ITurboGameState Methods ----------------------------------------------------------------------------------------
			virtual void SaveBoolean(std::string key, bool value);
			virtual void SaveInteger(std::string key, int value);
			virtual void SaveFloat(std::string key, float value);
			virtual void SaveVector(std::string key, TurboVector3D value);
			virtual void SaveString(std::string key, std::string value);

			virtual bool LoadBoolean(std::string key, bool defaultValue);
			virtual int LoadInteger(std::string key, int defaultValue);
			virtual float LoadFloat(std::string key, float defaultValue);
			virtual TurboVector3D LoadVector(std::string key, TurboVector3D defaultValue);
			virtual std::string LoadString(std::string key, std::string defaultValue);

		private:
			std::map<std::string, std::string> _map;

		};
	}
}