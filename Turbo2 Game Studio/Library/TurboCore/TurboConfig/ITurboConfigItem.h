
#pragma once

#include <pch.h>

#include <TurboConfigValueChangedEventArgs.h>
#include <TurboEvent.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class ITurboConfigItem
			{
			public:
				//  ITurboConfigItem Events ------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> OnValueChanged() = 0;
				
				//  ITurboConfigItem Properties --------------------------------------------------------------------
				virtual std::string GetName() = 0;
				virtual std::string GetFullName() = 0;

				virtual std::string GetAsString() = 0;
                virtual void SetAsString(const std::string& valueString) = 0;
				virtual bool IsEmpty() = 0;

				virtual std::string GetDefaultString() = 0;
				virtual void SetDefaultString(std::string value) = 0;

				virtual bool IsDefaultValue() = 0;
			};
		}
	}
}