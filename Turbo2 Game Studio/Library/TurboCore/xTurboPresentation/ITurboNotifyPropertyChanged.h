#pragma once

#include <pch.h>

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
				virtual std::vector<std::string> GetChangedProperties() = 0;
			};
		}
	}
}