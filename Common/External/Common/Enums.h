#pragma once

namespace CainEngine::Common {

/*
	Severity of the logging
*/
enum class LogSeverity : uint8_t
{
	/* Fatal error, system is in a state of undefined behaviour */
	FatalError = 0,

	/* Invalid behaviour, but the system can continue */
	Error = 1,

	/* Possible invalid behaviour, but could be intentional */
	Warning = 2,

	/* Extra information */
	Message = 3
};

}; // namespace CainEngine::Common
