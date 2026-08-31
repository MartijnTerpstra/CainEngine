#include "Precomp.h"

#include <mdebug.h>
#include <mstacktrace.h>

namespace CainEngine {

namespace {

void defaultFatalError(std::string_view)
{
	printf("Callstack:\n");

	for(auto& line : mst::stacktrace::current())
	{
		std::cout << line << std::endl;
	}

	std::cout << std::endl << "Press enter to exit" << std::endl;
	std::string s;
	std::getline(std::cin, s);
	if(s == "b" || s == "break")
		MST_BREAKPOINT;
	exit(1);
}

constinit FatalErrorHandlerFn g_onFatalError = defaultFatalError;

} // namespace

// Declared directly under CainEngine (not CainEngine::Common) in Common.h -
// this definition has to match that exactly, or it silently compiles as an
// unrelated CainEngine::Common::setFatalErrorHandler that's never linked
// against the actual declaration (no compiler/linker error until something
// finally calls it).
void setFatalErrorHandler(FatalErrorHandlerFn handler)
{
	g_onFatalError = handler ? handler : defaultFatalError;
}

namespace Common {

[[noreturn]] void Details::invokeFatalErrorHandler(std::string_view str)
{
	g_onFatalError(str);
	throw std::runtime_error("FatalErrorHandler should not return");
}

#if DEBUG_CHECKS
void unreachable()
{
	Details::invokeFatalErrorHandler("Unreachable code path");
}
#endif

} // namespace Common

} // namespace CainEngine

void mst::fatalError(std::string_view str)
{
	CainEngine::Common::fatalError(str);
}
