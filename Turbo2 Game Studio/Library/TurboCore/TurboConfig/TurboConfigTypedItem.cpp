
#include <pch.h>

#include <TurboConfigTypedItem.h>

#include <utility>

using namespace Turbo::Core::Config;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

template <typename TValue>
TurboConfigTypedItem<TValue>::TurboConfigTypedItem(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboConfigProvider> provider,
	std::string name,
	TValue defaultValue) :
		_debug(std::move(debug)),
		_provider(std::move(provider)),
		_name(std::move(name)),
		_value(defaultValue)
{
	SetDefaultValue(defaultValue);

	_onValueChanged = std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>>
	        (new TurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>());
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboConfigItem Properties ----------------------------------------------------------------------------------------

template <typename TValue>
std::string TurboConfigTypedItem<TValue>::GetFullName()
{
	if (_fullName.empty())
	{
		_fullName = _provider->GetFullName() + "." + _name;
	}
	return _fullName;
}

template <typename TValue>
std::string TurboConfigTypedItem<TValue>::GetAsString()
{
	bool providerIsInitialized = _provider->IsInitialized();

	if  (!_isInitialized)
	{
		if (!providerIsInitialized)
		{
			_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetAsString: return (default) \"" << _defaultString << "\"\n";
			return _defaultString;
		}

		_valueString = _provider->ReadString(GetName());
		_value = ConvertStringToValue(_valueString, _defaultValue);
		_valueString = ConvertValueToString(_value);

		_isInitialized = true;
		NotifyValueChanged();
	}

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetAsString: return \"" << _valueString << "\"\n";

	return _valueString;
}

template <typename TValue>
void TurboConfigTypedItem<TValue>::SetAsString(const std::string& valueString)
{
	bool providerIsInitialized = _provider->IsInitialized();

	if (!providerIsInitialized)
	{
		return;
	}

	if ((_valueString == valueString) && _isInitialized)
	{
		return;
	}

	_valueString = valueString;
	_value = ConvertStringToValue(_valueString, _defaultValue);

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": SetAsString: \"" << _valueString << "\" => " << _value << "\n";

	_provider->WriteString(GetName(), _valueString, IsDefaultValue());
	_isInitialized = true;
	NotifyValueChanged();
}

//  ITurboConfigItem Properties ----------------------------------------------------------------------------------------
//  ITurboConfigTypedItem Properties -----------------------------------------------------------------------------------

template <typename TValue>
TValue TurboConfigTypedItem<TValue>::GetValue()
{
	bool providerIsInitialized = _provider->IsInitialized();

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetValue: " << _isInitialized << ", " << providerIsInitialized << "\n";

	if  (!_isInitialized)
	{
		if (!providerIsInitialized)
		{
			_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetValue: return (default) " << _defaultValue << "\n";

			return _defaultValue;
		}

		_valueString = _provider->ReadString(GetName());
		_value = ConvertStringToValue(_valueString, _defaultValue);
		_valueString = ConvertValueToString(_value);

		_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetValue: \"" << _valueString << "\", " << _value << "\n";

		_isInitialized = true;
		NotifyValueChanged();
	}

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": GetValue: return " << _value << "\n";

	return _value;
}

template <typename TValue>
void TurboConfigTypedItem<TValue>::SetValue(TValue value)
{
	bool providerIsInitialized = _provider->IsInitialized();

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": SetValue(" << value << "): " << _isInitialized << ", " << providerIsInitialized << "\n";

	if (!providerIsInitialized)
	{
		return;
	}

	if ((value == _value) && _isInitialized)
	{
		return;
	}

	_value = value;
	_valueString = ConvertValueToString(_value);

	_debug->Send(debugVerbose, debugConfig) << GetFullName() << ": SetValue: " << _value << " => \"" << _valueString << "\"\n";

	_provider->WriteString(GetName(), _valueString, IsDefaultValue());
	_isInitialized = true;
	NotifyValueChanged();
}

template <typename TValue>
void TurboConfigTypedItem<TValue>::SetDefaultValue(TValue defaultValue)
{
	_defaultValue = defaultValue;
	_defaultString = ConvertValueToString(_defaultValue);

	_value = _defaultValue;
	_valueString = _defaultString;
}

//  ITurboConfigItem Properties ----------------------------------------------------------------------------------------
//  ITurboConfigItem Methods -------------------------------------------------------------------------------------------

template <typename TValue>
TValue TurboConfigTypedItem<TValue>::ConvertStringToValue(std::string valueString, TValue defaultValue)
{
	if (valueString.empty())
	{
		return defaultValue;
	}

	std::stringstream valueStream(valueString);

	TValue value;

	valueStream >> value;

	return value;
}

template <typename TValue>
std::string TurboConfigTypedItem<TValue>::ConvertValueToString(TValue value)
{
	std::stringstream valueStream;

	valueStream << value;

	std::string valueString;

	valueStream >> valueString;

	return valueString;
}

//  ITurboConfigItem Methods -------------------------------------------------------------------------------------------
//  Private Methods ----------------------------------------------------------------------------------------------------

template <typename TValue>
void TurboConfigTypedItem<TValue>::NotifyValueChanged()
{
	auto eventArgs = std::shared_ptr<TurboConfigValueChangedEventArgs>(new TurboConfigValueChangedEventArgs(
			GetName(), _valueString, _previousString));

	_previousString = _valueString;

	OnValueChanged()->Publish(this, eventArgs);
}

//  Private Methods ----------------------------------------------------------------------------------------------------

template class Turbo::Core::Config::TurboConfigTypedItem<int>;
template class Turbo::Core::Config::TurboConfigTypedItem<bool>;