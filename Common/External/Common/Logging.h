#pragma once

#include <cstdio>
#include <string_view>

namespace CainEngine::Common {

void Log(LogSeverity severity, std::string str);

template<typename... Args>
[[noreturn]] constexpr inline void FatalError(const char* formatStr, Args&&... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Log(LogSeverity::FatalError, formattedString);
}

template<typename... Args>
constexpr inline void Error(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Log(LogSeverity::Error, formattedString);
}

template<typename... Args>
constexpr inline void Warning(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Log(LogSeverity::Warning, formattedString);
}

template<typename... Args>
constexpr inline void Message(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Log(LogSeverity::Message, formattedString);
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

} // namespace CainEngine::Common