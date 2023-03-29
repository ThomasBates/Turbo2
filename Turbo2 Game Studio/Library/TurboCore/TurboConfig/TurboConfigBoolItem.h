
#pragma once

#include <pch.h>

#include <TurboConfigTypedItem.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class TurboConfigBoolItem : public TurboConfigTypedItem<bool>
			{
			public:
				TurboConfigBoolItem(
					std::shared_ptr<ITurboDebug> debug,
					std::shared_ptr<ITurboConfigProvider> provider,
					std::string name,
					bool defaultValue);
				virtual ~TurboConfigBoolItem() {}

				//  ITurboConfigBoolItem Methods -------------------------------------------------------------------
				virtual bool ConvertStringToValue(std::string valueString, bool defaultValue);
				virtual std::string ConvertValueToString(bool value);
			};
		}
	}
}