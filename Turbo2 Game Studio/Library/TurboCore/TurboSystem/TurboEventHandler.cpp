#include <pch.h>

#include <TurboEventHandler.h>

using namespace Turbo::Core::System;

template <class TReceiver, typename TArgs>
TurboEventHandler<TReceiver, TArgs>::TurboEventHandler(TReceiver *receiver, EventHandlerCallback eventHandlerCallback) :
    _receiver(receiver),
    _eventHandlerCallback(eventHandlerCallback)
{
}

template<class TReceiver, typename TArgs>
void TurboEventHandler<TReceiver, TArgs>::PublishEvent(void *sender, TArgs args)
{
    if (_receiver != nullptr && _eventHandlerCallback != nullptr)
        (_receiver->*_eventHandlerCallback)(sender, args);
}
