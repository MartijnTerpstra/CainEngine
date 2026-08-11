#include <Common.h>
#include <CommonTestSupport.h>

#include <gtest/gtest.h>

#include <cstring>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;
using namespace ::CainEngine::TestSupport;

// Without FatalErrorTest installing a handler, any of these would hit the
// default handler and hang the whole test binary waiting on std::cin.

TEST_F(FatalErrorTest, FatalErrorThrowsInsteadOfBlocking)
{
	EXPECT_THROW(Common::FatalError("deliberate failure: %d", 42), FatalErrorTriggered);
}

TEST_F(FatalErrorTest, FatalErrorMessageIsPreservedOnTheException)
{
	try
	{
		Common::FatalError("deliberate failure: %d", 42);
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
