#pragma once

#include <cstdio>
#include <string_view>

namespace CainEngine::Common {

namespace Details {

[[noreturn]] void InvokeFatalErrorHandler(std::string_view str);

void Log(LogSeverity severity, std::string_view str);

} // namespace Details

template<typename... Args>
[[noreturn]] constexpr inline void FatalError(const char* formatStr, Args&&... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::Log(LogSeverity::FatalError, formattedString);
	Details::InvokeFatalErrorHandler(formattedString);
}

[[noreturn]] constexpr inline void FatalError(std::string_view str)
{
	Details::Log(LogSeverity::FatalError, str);
	Details::InvokeFatalErrorHandler(str);
}

template<typename... Args>
constexpr inline void Error(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::Log(LogSeverity::Error, formattedString);
}

template<typename... Args>
constexpr inline void Warning(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::Log(LogSeverity::Warning, formattedString);
}

template<typename... Args>
constexpr inline void Message(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::Log(LogSeverity::Message, formattedString);
}

class IConsole
{
	COMMON_DECLARE_INTERFACE(IConsole);

public:
	// Main functionality

	virtual void WriteLine(LogSeverity severity, std::string_view value) = 0;
};

inline IConsole::~IConsole() = default;

void SetConsole(IConsole* console);

} // namespace CainEngine::Common