#pragma once

#include <pch.h>
#include <streambuf>

#include <ITurboDebug.h>
#include <TurboDebugStream.h>

namespace Turbo
{
	namespace Core
	{
		namespace Debug
		{
			class TurboDebug : public ITurboDebug
			{
			public:
				TurboDebug(std::shared_ptr<ITurboDebugLogger> logger);
				virtual ~TurboDebug(){}

				virtual void Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message);
				virtual TurboDebugStream& Send(TurboDebugSeverity severity, TurboDebugCategory category);

				virtual std::shared_ptr<ITurboDebugLogger> Logger() { return _logger; }
				virtual void Logger(std::shared_ptr<ITurboDebugLogger> logger) { _logger = logger; _stream.Logger(logger); }

			private:
				TurboDebugStream _stream;
				TurboDebugStream _nullStream;
				std::shared_ptr<ITurboDebugLogger> _logger;
			};
		}
	}
}