#pragma once

#include <pch.h>

#include <ITurboEventHandler.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <class TReceiver, typename TArgs>
			class TurboEventHandler : public ITurboEventHandler<TArgs>
			{
			public:
                typedef void (TReceiver::*EventHandlerCallback)(void*, TArgs);

			    TurboEventHandler(TReceiver *receiver, EventHandlerCallback eventHandlerCallback) :
						_receiver(receiver),
						_eventHandlerCallback(eventHandlerCallback)
				{
				}
			    virtual ~TurboEventHandler(){}

				virtual void PublishEvent(void *sender, TArgs args)
				{
					if (_receiver != nullptr && _eventHandlerCallback != nullptr)
						(_receiver->*_eventHandlerCallback)(sender, args);
				}


			private:
			    TReceiver *_receiver;
			    EventHandlerCallback _eventHandlerCallback;
			};
		}
	}
}