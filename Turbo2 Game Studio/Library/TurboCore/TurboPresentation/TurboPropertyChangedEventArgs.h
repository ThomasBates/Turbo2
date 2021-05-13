#pragma once

#include <pch.h>

#include <TurboEventArgs.h>

using namespace Turbo::Core::System;

namespace Turbo
{
	namespace Core
	{
		namespace Presentation
		{
			class TurboPropertyChangedEventArgs : public TurboEventArgs
			{
			public:
			    TurboPropertyChangedEventArgs(std::string propertyName) : _propertyName(propertyName) {}
			    virtual ~TurboPropertyChangedEventArgs(){}

			    std::string PropertyName() { return _propertyName; }
			private:
			    std::string _propertyName;
			};
		}
	}
}