#include <pch.h>

#include <TurboEvent.h>

using namespace Turbo::Core::System;

template<typename TArgs>
void TurboEvent<TArgs>::Subscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler)
{
    //  eventHandler is already subscribed? Don't resubscribe.
    if (_eventHandlers.find(eventHandler) != _eventHandlers.end())
        return;

    _eventHandlers.push_back(eventHandler);
}

template<typename TArgs>
void TurboEvent<TArgs>::Unsubscribe(std::shared_ptr<ITurboEventHandler<TArgs>> eventHandler)
{
    auto iterator = _eventHandlers.find(eventHandler);

    //  eventHandler is not subscribed? Don't unsubscribe.
    if (iterator != _eventHandlers.end())
        _eventHandlers.erase(iterator);
}

template<typename TArgs>
void TurboEvent<TArgs>::Publish(void *sender, TArgs args)
{
    for(auto eventHandler : _eventHandlers)
        if (eventHandler != nullptr)
            eventHandler->PublishEvent(sender, args);
}
