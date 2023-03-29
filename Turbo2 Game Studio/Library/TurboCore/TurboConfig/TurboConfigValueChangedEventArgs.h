#pragma once

#include <pch.h>

#include <TurboEventArgs.h>

using namespace Turbo::Core::System;

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class TurboConfigValueChangedEventArgs : public TurboEventArgs
			{
			public:
			    TurboConfigValueChangedEventArgs(std::string name, std::string value, std::string oldValue) :
					_name(name),
					_value(value),
					_oldValue(oldValue)
				{}
			    virtual ~TurboConfigValueChangedEventArgs(){}

			    std::string GetName() { return _name; }
			    std::string GetValue() { return _value; }
			    std::string GetOldValue() { return _oldValue; }
			private:
			    std::string _name;
			    std::string _value;
			    std::string _oldValue;
			};
		}
	}
}