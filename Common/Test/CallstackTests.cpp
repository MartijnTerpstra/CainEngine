#include <Common.h>

#include <gtest/gtest.h>

// COMMON_CALLSTACK_CALL only records anything when COMMON_MONITOR_CALLSTACK is
// enabled, which Callstack.h defaults on for _DEBUG builds - the configuration
// these tests run in. In a non-debug build the macro expands to nothing and
// every test below would see an empty stack.

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

std::vector<std::string> captureFromInnerScope()
{
	COMMON_CALLSTACK_CALL;
	return Callstack::get();
}

std::vector<std::string> captureFromNestedScopes()
{
	COMMON_CALLSTACK_CALL;
	return captureFromInnerScope();
}

} // namespace

TEST(Callstack, EmptyOutsideAnyTrackedScope)
{
	EXPECT_TRUE(Callstack::get().empty());
}

TEST(Callstack, SingleScopeRecordsTheCallingFunction)
{
	auto stack = captureFromInnerScope();

	ASSERT_EQ(1u, stack.size());
	EXPECT_NE(std::string::npos, stack[0].find("captureFromInnerScope"));
}

TEST(Callstack, NestedScopesAreOrderedInnermostFirst)
{
	auto stack = captureFromNestedScopes();

	ASSERT_EQ(2u, stack.size());
	EXPECT_NE(std::string::npos, stack[0].find("captureFromInnerScope"));
	EXPECT_NE(std::string::npos, stack[1].find("captureFromNestedScopes"));
}

TEST(Callstack, DepthReturnsToZeroAfterScopesExit)
{
	captureFromNestedScopes();

	EXPECT_TRUE(Callstack::get().empty());
}
