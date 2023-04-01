
#include <pch.h>

#include <TurboConfigBoolItem.h>
#include <TurboConfigTypedItem.h>

using namespace Turbo::Core::Config;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TurboConfigBoolItem::TurboConfigBoolItem(
	std::shared_ptr<ITurboDebug> debug,
	std::shared_ptr<ITurboConfigProvider> provider,
	std::string name,
	bool defaultValue) :
		TurboConfigTypedItem<bool>(std::move(debug), std::move(provider), std::move(name), defaultValue)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboConfigItem Methods -------------------------------------------------------------------------------------------

bool TurboConfigBoolItem::ConvertStringToValue(std::string valueString, bool defaultValue)
{
	if (valueString == "true")
	{
		return true;
	}

	if(valueString == "false")
	{
		return false;
	}

	return defaultValue;
}

std::string TurboConfigBoolItem::ConvertValueToString(bool value)
{
	return value ? "true" : "false";
}

//  ITurboConfigItem Methods -------------------------------------------------------------------------------------------
