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
			private:
				std::vector<std::shared_ptr<ITurboEventHandler<TArgs>>> _eventHandlers;

			public:
			    TurboEvent(){}
			    virtual ~TurboEvent(){}

				virtual void Subscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler)
				{
					//  eventHandler is already subscribed? Don't resubscribe.
					if (std::find(_eventHandlers.begin(), _eventHandlers.end(), eventHandler) != _eventHandlers.end())
						return;

					_eventHandlers.push_back(eventHandler);
				}

				virtual void Unsubscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler)
				{
					_eventHandlers.erase(std::remove(_eventHandlers.begin(), _eventHandlers.end(), eventHandler), _eventHandlers.end());
				}

				virtual void Publish(void *sender, TArgs args)
				{
					for(auto eventHandler : _eventHandlers)
						if (eventHandler != nullptr)
							eventHandler->PublishEvent(sender, args);
				}

				virtual int Count()
				{
			    	return _eventHandlers.size();
				}
			};
        }
	}
}