#pragma once

#include <cstdio>
#include <string_view>

namespace CainEngine::Common {

namespace Details {

[[noreturn]] void invokeFatalErrorHandler(std::string_view str);

void log(LogSeverity severity, std::string_view str);

} // namespace Details

template<typename... Args>
[[noreturn]] constexpr inline void fatalError(const char* formatStr, Args&&... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::log(LogSeverity::FatalError, formattedString);
	Details::invokeFatalErrorHandler(formattedString);
}

[[noreturn]] constexpr inline void fatalError(std::string_view str)
{
	Details::log(LogSeverity::FatalError, str);
	Details::invokeFatalErrorHandler(str);
}

template<typename... Args>
constexpr inline void error(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::log(LogSeverity::Error, formattedString);
}

template<typename... Args>
constexpr inline void warning(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::log(LogSeverity::Warning, formattedString);
}

template<typename... Args>
constexpr inline void message(const char* formatStr, Args... args)
{
	auto formattedString = mst::to_printf_string(formatStr, std::forward<Args>(args)...);

	Details::log(LogSeverity::Message, formattedString);
}

class IConsole
{
	COMMON_DECLARE_INTERFACE(IConsole);

public:
	// Main functionality

	virtual void writeLine(LogSeverity severity, std::string_view value) = 0;
};

inline IConsole::~IConsole() = default;

void setConsole(IConsole* console);

} // namespace CainEngine::Common