#pragma once

#include <ITurboSceneVertexShader.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneVertexShader : public ITurboSceneVertexShader
		{
		public:
			//  Constructors ---------------------------------------------------------------------------------------------------
			TurboSceneVertexShader(std::string name);
			virtual ~TurboSceneVertexShader() {};

			//  ITurboSceneVertexShader Properties -----------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

		private:
			std::string _name;
		};
	}
}