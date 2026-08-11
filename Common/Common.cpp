#include "Precomp.h"

#include "FatalErrorHandler.hpp"
#include <mdebug.h>

namespace CainEngine {

namespace {

void DefaultFatalError(std::string_view str)
{
	std::cout << str << std::endl;
	printf("Callstack:\n");
	for(auto& line : Common::Callstack::Get())
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

constinit FatalErrorHandlerFn g_onFatalError = DefaultFatalError;

}

// Declared directly under CainEngine (not CainEngine::Common) in Common.h -
// this definition has to match that exactly, or it silently compiles as an
// unrelated CainEngine::Common::SetFatalErrorHandler that's never linked
// against the actual declaration (no compiler/linker error until something
// finally calls it).
void SetFatalErrorHandler(FatalErrorHandlerFn handler)
{
	g_onFatalError = handler ? handler : DefaultFatalError;
}

namespace Common {

void InvokeFatalErrorHandler(std::string_view str)
{
	g_onFatalError(str);
}

#if DEBUG_CHECKS
void Unreachable()
{
	InvokeFatalErrorHandler("Unreachable code path");
}
#endif

} // namespace Common

} // namespace CainEngine

void mst::fatalError(std::string_view str)
{
	CainEngine::Common::InvokeFatalErrorHandler(str);
}
