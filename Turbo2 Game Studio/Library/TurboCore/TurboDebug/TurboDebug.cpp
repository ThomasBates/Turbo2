
#include <pch.h>

#include <TurboDebug.h>

using namespace Turbo::Core::Debug;

TurboDebug::TurboDebug(std::shared_ptr<ITurboDebugLogger> logger) :
	_logger(logger)
{
	_stream.Logger(_logger);
}

void TurboDebug::Send(TurboDebugSeverity severity, TurboDebugCategory category, std::string message)
{
	if (_logger == nullptr)
	{
		return;
	}

	if (category != debugController)
	{
		return;
	}

	_logger->Log(DebugSeverity(severity), DebugCategory(category), message);
}

TurboDebugStream& TurboDebug::Send(TurboDebugSeverity severity, TurboDebugCategory category)
{
	if (category != debugController)
	{
		return _nullStream;
	}

	_stream.Severity(DebugSeverity(severity));
	_stream.Category(DebugCategory(category));

	return _stream;
}
