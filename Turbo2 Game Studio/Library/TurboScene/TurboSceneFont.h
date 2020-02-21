#pragma once

#include <pch.h>

#include <ITurboSceneFont.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneFont : public ITurboSceneFont
		{
		public:
			//  Constructors -------------------------------------------------------------------------------------------
			TurboSceneFont(std::string name);

			//  ITurboSceneObjectFont Properties -----------------------------------------------------------------------
			virtual std::string Name() { return _name; }

		private:
			std::string _name;
		};
	}
}