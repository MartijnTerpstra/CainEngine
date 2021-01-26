#pragma once

#ifndef COMMON_MONITOR_CALLSTACK
#if _DEBUG
#define COMMON_MONITOR_CALLSTACK 1
#endif
#endif

#if COMMON_MONITOR_CALLSTACK
#define COMMON_CALLSTACK_CALL ::Common::CallstackScope _MST_ANONYMOUS_VAR {__FUNCTION__, __FILE__, __LINE__}
#else
#define COMMON_CALLSTACK_CALL
#endif

namespace Common {

class CallstackScope
{
public:
	CallstackScope(const char* function, const char* file, int line);
	~CallstackScope();
};

/**
	Summary:
		A api callstack, for tracing and debugging purposes
*/
class Callstack
{
public:

	static vector<string> Get();

};

}; // namespace Common