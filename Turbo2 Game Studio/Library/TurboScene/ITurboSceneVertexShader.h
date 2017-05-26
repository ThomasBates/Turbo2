#pragma once

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneVertexShader
		{
		public:
			//  ITurboSceneVertexShader Properties -----------------------------------------------------------------------------
			virtual std::string Name() = 0;
		};
	}
}