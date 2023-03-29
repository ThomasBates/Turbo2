
#pragma once

#include <pch.h>

#include <ITurboConfigItem.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			template<typename TValue>
			class ITurboConfigTypedItem : public ITurboConfigItem
			{
			public:
				//  ITurboConfigTypedItem Properties -------------------------------------------------------------------
				virtual TValue GetValue() = 0;
				virtual void SetValue(TValue value) = 0;

				virtual TValue GetDefaultValue() = 0;
				virtual void SetDefaultValue(TValue defaultValue) = 0;

				//  ITurboConfigTypedItem Methods ----------------------------------------------------------------------
				virtual TValue ConvertStringToValue(std::string valueString, TValue defaultValue) = 0;
				virtual std::string ConvertValueToString(TValue value) = 0;
			};
		}
	}
}