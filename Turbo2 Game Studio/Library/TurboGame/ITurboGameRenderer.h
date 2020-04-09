
#pragma once

#include <ITurboSceneFont.h>
#include <ITurboView.h>
#include <ITurboViewRendererAccess.h>

using namespace Turbo::View;

namespace Turbo
{
	namespace Game
	{
		class ITurboGameRenderer
		{
		public:
			//	ITurboGameRenderer Properties ----------------------------------------------------------------------
			virtual std::shared_ptr<ITurboViewRendererAccess> RendererAccess() = 0;

			//	ITurboGameRenderer Methods -----------------------------------------------------------------------------
			virtual void RegisterFont(std::shared_ptr<ITurboSceneFont> font) = 0;
			virtual void UpdateDisplayInformation() = 0;
			virtual bool LoadView(std::shared_ptr<ITurboView> view) = 0;
			virtual bool RenderView(std::shared_ptr<ITurboView> view) = 0;
			virtual void Reset() = 0;
		};
	}
}
