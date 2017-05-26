
#pragma once

#include <pch.h>
#include <TurboVector3D.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameState
		{
		public:
			//  ITurboGameState Properties -------------------------------------------------------------------------------------
			virtual std::vector<std::string> Keys() = 0;

			//  ITurboGameState Methods ----------------------------------------------------------------------------------------
			virtual void SaveBoolean(std::string key, bool value) = 0;
			virtual void SaveInteger(std::string key, int value) = 0;
			virtual void SaveFloat(std::string key, float value) = 0;
			virtual void SaveVector(std::string key, TurboVector3D value) = 0;
			virtual void SaveString(std::string key, std::string value) = 0;

			virtual bool LoadBoolean(std::string key) = 0;
			virtual int LoadInteger(std::string key) = 0;
			virtual float LoadFloat(std::string key) = 0;
			virtual TurboVector3D LoadVector(std::string key) = 0;
			virtual std::string LoadString(std::string key) = 0;
		};
	}
}