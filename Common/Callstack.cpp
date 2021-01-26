#include "Precomp.h"

using namespace Common;

static const size_t STACK_SIZE = 128;

static thread_local unsigned long g_StackIdx = 0;

typedef ::std::tuple<const char*, const char*, int>(&StackType)[STACK_SIZE];

static StackType& GetCallstack()
{
	static thread_local ::std::tuple<const char*, const char*, int> stack[STACK_SIZE];

	return stack;
}

CallstackScope::CallstackScope(const char * function, const char * file, int line)
{ 
	auto index = g_StackIdx++;
	if(index < STACK_SIZE)
	{
		GetCallstack()[index] = { function, file, line };
	}
}

CallstackScope::~CallstackScope()
{
	--g_StackIdx;
}

vector<string> Callstack::Get()
{
	vector<string> retval;

	auto index = g_StackIdx;

	while(--index < STACK_SIZE)
	{
		auto& line = GetCallstack()[index];

		string func = std::get<0>(line);
		/*
		size_t foundPos;
		if((foundPos = func.find(" __cdecl ")) != string::npos)
		{
			func = func.substr(foundPos + 8);
		}
		else if((foundPos = func.find(" __stdcall ")) != string::npos)
		{
			func = func.substr(foundPos + 8);
		}
		else if((foundPos = func.find(" __fastcall ")) != string::npos)
		{
			func = func.substr(foundPos + 8);
		}
		else if((foundPos = func.find(" __vectorcall ")) != string::npos)
		{
			func = func.substr(foundPos + 8);
		}
		*/

		retval.push_back(func + " at " + std::get<1>(line) + ":line " + std::to_string(std::get<2>(line)));
	}

	return retval;
}