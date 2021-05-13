#pragma once

#include <pch.h>

#include <ITurboNotifyPropertyChanged.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			class TurboNotifyPropertyChanged : public ITurboNotifyPropertyChanged
			{
			public:
				TurboNotifyPropertyChanged();
				virtual ~TurboNotifyPropertyChanged() {}

				//  virtual event NotifyPropertyChanged
				//virtual std::vector<std::string> GetChangedProperties() = 0;
				virtual std::shared_ptr<TurboEvent<std::shared_ptr<TurboPropertyChangedEventArgs>>> OnPropertyChanged() { return _onPropertyChanged; }

			protected:
				void NotifyPropertyChanged(std::string propertyName);

			private:
				std::shared_ptr<TurboEvent<std::shared_ptr<TurboPropertyChangedEventArgs>>> _onPropertyChanged;
			};
		}
	}
}