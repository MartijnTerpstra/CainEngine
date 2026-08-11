#include <Common.h>

#include <gtest/gtest.h>

// COMMON_CALLSTACK_CALL only records anything when COMMON_MONITOR_CALLSTACK is
// enabled, which Callstack.h defaults on for _DEBUG builds - the configuration
// these tests run in. In a non-debug build the macro expands to nothing and
// every test below would see an empty stack.

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

std::vector<std::string> CaptureFromInnerScope()
{
	COMMON_CALLSTACK_CALL;
	return Callstack::Get();
}

std::vector<std::string> CaptureFromNestedScopes()
{
	COMMON_CALLSTACK_CALL;
	return CaptureFromInnerScope();
}

} // namespace

TEST(Callstack, EmptyOutsideAnyTrackedScope)
{
	EXPECT_TRUE(Callstack::Get().empty());
}

TEST(Callstack, SingleScopeRecordsTheCallingFunction)
{
	auto stack = CaptureFromInnerScope();

	ASSERT_EQ(1u, stack.size());
	EXPECT_NE(std::string::npos, stack[0].find("CaptureFromInnerScope"));
}

TEST(Callstack, NestedScopesAreOrderedInnermostFirst)
{
	auto stack = CaptureFromNestedScopes();

	ASSERT_EQ(2u, stack.size());
	EXPECT_NE(std::string::npos, stack[0].find("CaptureFromInnerScope"));
	EXPECT_NE(std::string::npos, stack[1].find("CaptureFromNestedScopes"));
}

TEST(Callstack, DepthReturnsToZeroAfterScopesExit)
{
	CaptureFromNestedScopes();

	EXPECT_TRUE(Callstack::Get().empty());
}
