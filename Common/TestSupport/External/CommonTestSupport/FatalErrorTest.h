#pragma once

namespace CainEngine::TestSupport {

/**
	Summary:
		Thrown by the fatal-error handler FatalErrorTest installs in place of
		the default handler, which blocks on std::cin waiting for a debugger
		break/exit choice.

	Notes:
		Letting this propagate out of a TEST_F body is enough on its own:
		googletest's default exception handling (--gtest_catch_exceptions,
		on by default) reports it as a test failure, printing what() - which
		holds the original Common::fatalError()/COMMON_ASSERT() message.
		Catch it explicitly (or use EXPECT_THROW/ASSERT_THROW) to assert that
		a specific operation is expected to hit a fatal error.
*/
class FatalErrorTriggered final : public std::exception
{
public:
	explicit FatalErrorTriggered(std::string message);

	const char* what() const noexcept override;

private:
	std::string m_message;
};

/**
	Summary:
		A gtest fixture base that replaces Common's fatal-error handler for
		the duration of each test.

	Notes:
		Without this, a COMMON_ASSERT() or Common::fatalError() triggered
		during a test hits the default handler, which prints a callstack and
		then blocks on std::cin waiting for a debugger break/exit choice -
		hanging the entire test binary instead of failing just the one test.

		Derive a fixture from this (or use it directly via TEST_F) to turn
		fatal errors into a normal, catchable test failure instead:

			TEST_F(FatalErrorTest, InvalidInputTriggersAnAssert)
			{
				EXPECT_THROW(SomeFunctionThatAsserts(), FatalErrorTriggered);
			}

		A test that doesn't trigger a fatal error behaves exactly as if this
		fixture weren't there - the handler is only ever invoked from within
		Common::fatalError()/COMMON_ASSERT(), never proactively.
*/
class FatalErrorTest : public ::testing::Test
{
protected:
	void SetUp() override;
	void TearDown() override;
};

} // namespace CainEngine::TestSupport
