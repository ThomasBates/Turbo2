#pragma once

#include <pch.h>

#include <TurboEvent.h>
#include <TurboPropertyChangedEventArgs.h>

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			class ITurboNotifyPropertyChanged
			{
			public:
				//  virtual event NotifyPropertyChanged
				//virtual std::vector<std::string> GetChangedProperties() = 0;
				virtual std::shared_ptr<TurboEvent<std::shared_ptr<TurboPropertyChangedEventArgs>>> OnPropertyChanged() = 0;
			};
		}
	}
}