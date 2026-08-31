#include <CommonTestSupport.h>

using namespace ::CainEngine;
using namespace ::CainEngine::TestSupport;

namespace {

[[noreturn]] void throwingFatalErrorHandler(std::string_view message)
{
	// Throwing is what actually satisfies Common::fatalError()'s [[noreturn]]
	// contract here (unlike a handler that just logs and returns, which
	// would be UB) - the throw unwinds the stack of whatever triggered the
	// fatal error, instead of letting it continue running past a broken
	// assumption.
	throw FatalErrorTriggered(std::string(message));
}

} // namespace

FatalErrorTriggered::FatalErrorTriggered(std::string message)
	: m_message(std::move(message))
{ }

const char* FatalErrorTriggered::what() const noexcept
{
	return m_message.c_str();
}

void FatalErrorTest::SetUp()
{
	setFatalErrorHandler(&throwingFatalErrorHandler);
}

void FatalErrorTest::TearDown()
{
	// setFatalErrorHandler(nullptr) restores Common's true default handler.
	// There's no public way to query/save the handler that was
	// active before SetUp() (setFatalErrorHandler has no getter), but since
	// gtest runs tests sequentially in one process, always resetting to the
	// documented default is equivalent in practice.
	setFatalErrorHandler(nullptr);
}
