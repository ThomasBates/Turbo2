
#pragma once

#include <pch.h>
#include <sstream>
#include <streambuf>

#include <ITurboDebugLogger.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class TurboDebugStreamBuf : public std::streambuf
			{
			public:
				// Construct a streambuf which sends output to the debug logger.
				TurboDebugStreamBuf()
				{
					_message = std::stringstream();
				}

				void Logger(std::shared_ptr<ITurboDebugLogger> logger)
				{
					_logger = logger;
				}

				void Severity(std::string severity)
				{
					_severity = severity;
				}

				void Category(std::string category)
				{
					_category = category;
				}

			private:
				// This debug buffer has no buffer. So every character "overflows"
				// and can be put directly into the debug logger.
				virtual int overflow(int c)
				{
					if (c == EOF)
					{
						return !EOF;
					}
					else
					{
						if (c == '\n')
						{
							if (_logger != nullptr)
							{
								_logger->Log(_severity, _category, _message.str());
							}
							_message = std::stringstream();
							return c;
						}
						else
						{
							int const r = _message.rdbuf()->sputc(c);
							return r == EOF ? EOF : c;
						}
					}
				}

			private:
				std::stringstream _message;
				std::shared_ptr<ITurboDebugLogger> _logger;
				std::string _severity;
				std::string _category;
			};
		}
	}
}