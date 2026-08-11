#include <Common.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;

TEST(PackChars, PacksFourCharsMostSignificantFirst)
{
	// 'a' = 0x61, 'b' = 0x62, 'c' = 0x63, 'd' = 0x64
	EXPECT_EQ(0x61626364u, Common::PackChars('a', 'b', 'c', 'd'));
}

TEST(PackChars, OrderingIsSignificant)
{
	EXPECT_NE(Common::PackChars('a', 'b', 'c', 'd'), Common::PackChars('d', 'c', 'b', 'a'));
}

TEST(PackChars, IsUsableAsAConstantExpression)
{
	constexpr uint32_t packed = Common::PackChars('d', 'x', '1', '1');

	EXPECT_EQ(0x64783131u, packed);
}
