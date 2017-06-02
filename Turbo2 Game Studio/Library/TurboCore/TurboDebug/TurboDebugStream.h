#pragma once

#include <pch.h>
#include <streambuf>

#include <TurboDebugStreamBuf.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class TurboDebugStream : public std::ostream
			{
			public:
				TurboDebugStream() :
					std::ostream(&_streambuf)
				{
				}

				void Logger(std::shared_ptr<ITurboDebugLogger> logger)
				{
					_streambuf.Logger(logger);
				}

				void Severity(std::string severity)
				{
					_streambuf.Severity(severity);
				}

				void Category(std::string category)
				{
					_streambuf.Category(category);
				}

			private:
				TurboDebugStreamBuf _streambuf;
			};
		}
	}
}