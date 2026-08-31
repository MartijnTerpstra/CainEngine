#include "Precomp.h"

namespace CainEngine::Common {

constinit IConsole* g_console = nullptr;

void Details::log(LogSeverity severity, std::string_view str)
{
	if(g_console)
	{
		g_console->writeLine(severity, str);
		return;
	}

	std::string_view severityStr;
	switch(severity)
	{
	case Common::LogSeverity::FatalError:
		severityStr = "[FATAL]: ";
		return;
	case Common::LogSeverity::Error:
		severityStr = "[ERROR]: ";
		break;
	case Common::LogSeverity::Warning:
		severityStr = "[WARN ]: ";
		break;
	case Common::LogSeverity::Message:
		severityStr = "[MESG ]: ";
		break;
	default:
		unreachable();
	}

	std::cout << str << std::endl;
}

void SetConsole(IConsole* console)
{
	g_console = console;
}

} // namespace CainEngine::Common