#include "Precomp.h"
#include "FatalErrorHandler.hpp"

namespace Common {

void Log(LogSeverity severity, string str)
{
	switch(severity)
	{
	case Common::LogSeverity::FatalError:
		str.insert((size_t)0, "[FATAL]: ");
		InvokeFatalErrorHandler(str);
		return;
	case Common::LogSeverity::Error:
		str.insert((size_t)0, "[ERROR]: ");
		break;
	case Common::LogSeverity::Warning:
		str.insert((size_t)0, "[WARN ]: ");
		break;
	case Common::LogSeverity::Message:
		str.insert((size_t)0, "[MESG ]: ");
		break;
	default:
		Unreachable();
	}

	std::cout << str << std::endl;
}

}