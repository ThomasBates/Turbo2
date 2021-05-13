
#include <pch.h>
#include <utility>

#include <TurboNotifyPropertyChanged.h>

using namespace Turbo::Core::Presentation;

TurboNotifyPropertyChanged::TurboNotifyPropertyChanged()
{
    _onPropertyChanged = std::shared_ptr<TurboEvent<std::shared_ptr<TurboPropertyChangedEventArgs>>>(new TurboEvent<std::shared_ptr<TurboPropertyChangedEventArgs>>());
}

void TurboNotifyPropertyChanged::NotifyPropertyChanged(std::string propertyName)
{
    auto eventArgs = std::shared_ptr<TurboPropertyChangedEventArgs>(new TurboPropertyChangedEventArgs(std::move(propertyName)));
    _onPropertyChanged->Publish(this, eventArgs);
}
