
#pragma once

#include <pch.h>

#include <ITurboView.h>
#include <ITurboViewRendererAccess.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace View
	{
		class TurboView : public virtual ITurboView
		{
		public:
			//	Constructors and Destructors ---------------------------------------------------------------------------
			TurboView(std::string name,
					  std::shared_ptr<ITurboViewRendererAccess> rendererAccess);
			virtual ~TurboView(){}

			//	ITurboView Properties ---------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

			virtual bool IsVisible() { return _isVisible; }
			virtual void IsVisible(bool isVisible);

			virtual TurboVector2D Position() { return _position; }
			virtual void Position(TurboVector2D position);

			virtual TurboVector2D Size() { return _size; }
			virtual void Size(TurboVector2D size);

			//	ITurboView Methods ------------------------------------------------------------------------------
			virtual bool Contains(float x, float y);

		protected:
			virtual void UpdateLayout(TurboVector2D position, TurboVector2D size) {}

			virtual void LoadScene(std::shared_ptr<ITurboScene> scene) { _rendererAccess->LoadScene(scene); }
			virtual void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) { _rendererAccess->LoadSceneSprite(sceneSprite); }
			virtual void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText) { _rendererAccess->LoadSceneText(sceneText); }

			virtual void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) { _rendererAccess->LoadSceneSound(sceneSound); }
			virtual void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) { _rendererAccess->LoadSceneBackground(sceneBackground); }

			virtual void RenderScene(std::shared_ptr<ITurboScene> scene) { _rendererAccess->RenderScene(scene); }
			virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite) { _rendererAccess->RenderSceneSprite(sceneSprite); }
			virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText) { _rendererAccess->RenderSceneText(sceneText); }

			virtual void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound) { _rendererAccess->RenderSceneSound(sceneSound); }
			virtual void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) { _rendererAccess->RenderSceneBackground(sceneBackground); }

		private:
			std::string _name;
			bool _isVisible = true;
			std::shared_ptr<ITurboViewRendererAccess> _rendererAccess;

			TurboVector2D _position = {};
			TurboVector2D _size = {};
		};
	}
}