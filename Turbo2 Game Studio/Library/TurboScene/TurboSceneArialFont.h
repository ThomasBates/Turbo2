
#pragma once

#include <pch.h>

#include <TurboSceneFont.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneArialFont : public TurboSceneFont
		{
		public:
			//  Constructors -------------------------------------------------------------------------------------------
			TurboSceneArialFont(std::shared_ptr<ITurboDebug> debug);
			virtual ~TurboSceneArialFont() {}
		};
	}
}