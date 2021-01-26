#pragma once

#include <cstdio>

namespace Common {

void Log(LogSeverity severity, string str);

void BreakOnLogSeverity(LogSeverity minimalSeverity);

template<typename... Args>
[[noreturn]] constexpr inline void FatalError(const char* formatStr, Args&&... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	::Common::Log(LogSeverity::FatalError, formattedString.c_str());

	exit(1);
}

template<typename... Args>
constexpr inline void Error(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	::Common::Log(LogSeverity::Error, formattedString.c_str());
}

template<typename... Args>
constexpr inline void Warning(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	::Common::Log(LogSeverity::Warning, formattedString.c_str());
}

template<typename... Args>
constexpr inline void Message(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	::Common::Log(LogSeverity::Message, formattedString.c_str());
}

class IConsole
{
	COMMON_DECLARE_INTERFACE(IConsole);
public:
	// Main functionality

	virtual void WriteLine(LogSeverity severity, const char* value) = 0;

};

inline IConsole::~IConsole() = default;

void SetConsole(IConsole* console);

} // namespace Common