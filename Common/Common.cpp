#include "Precomp.h"

#include "FatalErrorHandler.hpp"
#include <mdebug.h>

namespace Common {

namespace {

void DefaultFatalError(std::string_view str)
{
	std::cout << str << std::endl;
	printf("Callstack:\n");
	for(auto& line : Callstack::Get())
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

void SetFatalErrorHandler(FatalErrorHandlerFn handler)
{
	g_onFatalError = handler ? handler : DefaultFatalError;
}

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
}

void mst::fatalError(std::string_view str)
{
	Common::InvokeFatalErrorHandler(str);
}
