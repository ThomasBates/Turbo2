
#pragma once

#include <pch.h>

#include <ITurboView.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace View
	{
		class TurboView : public virtual ITurboView
		{
		public:
			//	Constructors and Destructors ---------------------------------------------------------------------------
			TurboView(std::string name);
			virtual ~TurboView(){}

			//	ITurboView Properties ---------------------------------------------------------------------------
			virtual std::string Name() { return _name; }

			virtual TurboVector2D Position() { return _position; }
			virtual void Position(TurboVector2D position);

			virtual TurboVector2D Size() { return _size; }
			virtual void Size(TurboVector2D size);

			//	ITurboView Methods ------------------------------------------------------------------------------
			virtual bool Contains(float x, float y);

		protected:
			virtual void UpdateLayout(TurboVector2D position, TurboVector2D size) {}

		private:
			std::string _name;
			TurboVector2D _position = {};
			TurboVector2D _size = {};
		};
	}
}