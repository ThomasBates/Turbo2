#pragma once

#include <pch.h>

#include <ITurboEventHandler.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <typename TArgs>
			class ITurboEvent
			{
			public:
				virtual void Subscribe(std::shared_ptr<ITurboEventHandler<TArgs>>) = 0;
				virtual void Unsubscribe(std::shared_ptr<ITurboEventHandler<TArgs>>) = 0;
				virtual void Publish(void *sender, TArgs args) = 0;
				virtual int Count() = 0;
			};
		}
	}
}