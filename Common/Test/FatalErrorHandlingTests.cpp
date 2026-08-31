#include <Common.h>
#include <CommonTestSupport.h>

#include <gtest/gtest.h>

#include <cstring>
#include <stdexcept>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;
using namespace ::CainEngine::TestSupport;

namespace {

// Deliberately violates the FatalErrorHandlerFn contract: a real handler is
// never supposed to return (it must throw, exit(), or otherwise terminate
// execution) - this simulates a client that fails to honor that [[noreturn]]
// policy.
void returningFatalErrorHandler(std::string_view)
{ }

} // namespace

// Without FatalErrorTest installing a handler, any of these would hit the
// default handler and hang the whole test binary waiting on std::cin.

TEST_F(FatalErrorTest, FatalErrorThrowsInsteadOfBlocking)
{
	EXPECT_THROW(Common::fatalError("deliberate failure: %d", 42), FatalErrorTriggered);
}

TEST_F(FatalErrorTest, FatalErrorMessageIsPreservedOnTheException)
{
	try
	{
		Common::fatalError("deliberate failure: %d", 42);
		FAIL() << "FatalError() should have thrown";
	}
	catch(const FatalErrorTriggered& caught)
	{
		EXPECT_NE(nullptr, std::strstr(caught.what(), "deliberate failure: 42"));
	}
}

TEST_F(FatalErrorTest, FailingAssertThrows)
{
	EXPECT_THROW(COMMON_ASSERT(1 == 2), FatalErrorTriggered);
}

TEST_F(FatalErrorTest, PassingAssertDoesNotThrow)
{
	EXPECT_NO_THROW(COMMON_ASSERT(1 == 1));
}

TEST_F(FatalErrorTest, FatalErrorThrowsRuntimeErrorWhenHandlerReturns)
{
	// FatalErrorTest's own handler (ThrowingFatalErrorHandler) always throws,
	// so it can't exercise this path - install a handler that breaks the
	// noreturn contract instead.
	setFatalErrorHandler(&returningFatalErrorHandler);

	EXPECT_THROW(Common::fatalError("deliberate failure: %d", 42), std::runtime_error);
}
