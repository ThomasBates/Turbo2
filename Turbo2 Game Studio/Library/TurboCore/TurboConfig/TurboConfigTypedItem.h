
#pragma once

#include <pch.h>

#include <ITurboConfigTypedItem.h>
#include <ITurboConfigProvider.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Debug;

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			template <typename TValue>
			class TurboConfigTypedItem : public ITurboConfigTypedItem<TValue>
			{
			public:
				TurboConfigTypedItem<TValue>(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboConfigProvider> provider,
					std::string name,
					TValue defaultValue);
				virtual ~TurboConfigTypedItem<TValue>() {}

				//  ITurboConfigItem Events ------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> OnValueChanged() { return _onValueChanged; }

				//  ITurboConfigItem Properties --------------------------------------------------------------------
				virtual std::string GetName() { return _name; }
				virtual std::string GetFullName();

				virtual std::string GetAsString();
				virtual void SetAsString(const std::string& valueString);
				virtual bool IsEmpty() { return _valueString.empty(); }

				virtual std::string GetDefaultString() { return _defaultString; }
				void SetDefaultString(std::string defaultString) { _defaultString = defaultString; }
				virtual bool IsDefaultValue() { return _valueString == _defaultString; }

				//  ITurboConfigTypedItem Properties ----------------------------------------------------------------
				virtual TValue GetValue();
				virtual void SetValue(TValue value);

				virtual TValue GetDefaultValue() { return _defaultValue; }
				virtual void SetDefaultValue(TValue defaultValue);

				//  ITurboConfigTypedItem Methods -------------------------------------------------------------------
				virtual TValue ConvertStringToValue(std::string valueString, TValue defaultValue);
				virtual std::string ConvertValueToString(TValue value);

			private:
				std::shared_ptr<ITurboDebug> _debug;
				std::shared_ptr<ITurboConfigProvider> _provider;
				std::string _name;
				std::string _fullName;

				std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> _onValueChanged;

				bool _isInitialized = false;
				std::string _valueString;
				std::string _previousString;

				std::string _defaultString;

				TValue _value;
				TValue _defaultValue;

				void NotifyValueChanged();
			};
		}
	}
}