#pragma once

#include <ITurboSceneTexture.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneTexture : public ITurboSceneTexture
		{
		public:
			//  Constructors ---------------------------------------------------------------------------------------------------
			TurboSceneTexture(std::string name);

			//  ITurboSceneObjectTexture Properties ----------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

		private:
			std::string _name;
		};
	}
}