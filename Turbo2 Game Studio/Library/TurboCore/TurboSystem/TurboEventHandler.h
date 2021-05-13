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

			    TurboEventHandler(TReceiver *receiver, EventHandlerCallback eventHandlerCallback);
			    virtual ~TurboEventHandler(){}

				virtual void PublishEvent(void *sender, TArgs args);

			private:
			    TReceiver *_receiver;
			    EventHandlerCallback _eventHandlerCallback;
			};
		}
	}
}