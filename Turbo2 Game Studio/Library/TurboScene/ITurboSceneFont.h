#pragma once

#include <pch.h>

#include <ITurboSceneMesh.h>
#include <ITurboSceneText.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneFont
		{
		public:
			//  ITurboSceneFont Properties -----------------------------------------------------------------------------
			virtual std::string Name() = 0;

			//  ITurboSceneFont Methods --------------------------------------------------------------------------------
			virtual std::shared_ptr<ITurboSceneMesh> CreateMesh(std::shared_ptr<ITurboSceneText> sceneText) = 0;
		};
	}
}