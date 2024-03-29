
#include <pch.h>

#include <TurboDebug.h>

using namespace Turbo::Core::Debug;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboDebug::TurboDebug(std::shared_ptr<ITurboDebugLogger> logger) :
	_logger(logger)
{
	_stream.Logger(_logger);
}
//  ITurboDebug Methods ------------------------------------------------------------------------------------------------

void TurboDebug::Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message)
{
	if (_logger == nullptr)
		return;

	if (severity < _severity)
		return;

	if (!CategoryEnabled(category))
		return;

	_logger->Log(DebugSeverity(severity), DebugCategory(category), message);
}

TurboDebugStream& TurboDebug::Send(TurboDebugSeverity severity, TurboDebugCategory category)
{
	if (_logger == nullptr)
		return _nullStream;


	if (severity < _severity)
		return _nullStream;

	if (!CategoryEnabled(category))
		return _nullStream;

	_stream.Severity(DebugSeverity(severity));
	_stream.Category(DebugCategory(category));

	return _stream;
}
