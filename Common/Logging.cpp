#include "Precomp.h"

static Common::LogSeverity g_minimalSeverityForBreak = Common::LogSeverity::FatalError;

void ::Common::Log(LogSeverity severity, string str)
{
	switch (severity)
	{
	case Common::LogSeverity::FatalError:
		str.insert((size_t)0, "[FATAL]: ");
		break;
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
		FatalError("severity: corrupted value");
		return;
	}

	std::cout << str << std::endl;

	if (severity <= g_minimalSeverityForBreak)
	{
		printf("Callstack:\n");
		for (auto& line : Callstack::Get())
		{
			std::cout << line << std::endl;
		}

		std::cout << std::endl << "Press enter to exit" << std::endl;
		std::string s;
		std::getline(std::cin, s);
		if (s == "b" || s == "break")
			_MST_BREAK;
		else if (s == "exit" || s == "quit")
			exit(1);
	}

}

void ::Common::BreakOnLogSeverity(Common::LogSeverity minimalSeverity)
{
	g_minimalSeverityForBreak = minimalSeverity;
}