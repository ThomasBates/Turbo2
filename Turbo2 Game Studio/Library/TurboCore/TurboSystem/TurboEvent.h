#pragma once

#include <pch.h>

#include <ITurboEvent.h>
#include <ITurboEventHandler.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <typename TArgs>
			class TurboEvent : public ITurboEvent<TArgs>
			{
			public:
			    TurboEvent(){}
			    virtual ~TurboEvent(){}

				virtual void Subscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler);
				virtual void Unsubscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler);
				virtual void Publish(void *sender, TArgs args);
				virtual int Count() { return _eventHandlers.size(); }

			private:
				std::vector<std::shared_ptr<ITurboEventHandler<TArgs>>> _eventHandlers;
			};
        }
	}
}