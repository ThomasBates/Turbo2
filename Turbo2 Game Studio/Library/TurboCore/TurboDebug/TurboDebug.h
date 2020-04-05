
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
				//  Constructors ---------------------------------------------------------------------------------------
				TurboDebug(std::shared_ptr<ITurboDebugLogger> logger);
				virtual ~TurboDebug(){}

				//  ITurboDebug Properties -----------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboDebugLogger> Logger() { return _logger; }
				virtual void Logger(std::shared_ptr<ITurboDebugLogger> logger) { _logger = logger; _stream.Logger(logger); }

				virtual TurboDebugSeverity Severity() { return _severity; }
				virtual void Severity(TurboDebugSeverity severity) { _severity = severity; }

				virtual bool CategoryEnabled(TurboDebugCategory category) { return _categoryEnabled.count(category) > 0 ? _categoryEnabled[category] : false; }
				virtual void CategoryEnabled(TurboDebugCategory category, bool enabled) { _categoryEnabled[category] = enabled; }

				//  ITurboDebug Methods --------------------------------------------------------------------------------
				virtual void Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message);
				virtual TurboDebugStream& Send(TurboDebugSeverity severity, TurboDebugCategory category);

			private:
				TurboDebugSeverity _severity = TurboDebugSeverity::debugDebug;
				std::map<TurboDebugCategory, bool> _categoryEnabled {};
				TurboDebugStream _stream;
				TurboDebugStream _nullStream;
				std::shared_ptr<ITurboDebugLogger> _logger;
			};
		}
	}
}