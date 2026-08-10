#include <EntitySystem.h>

#include <gtest/gtest.h>

using namespace ::CainEngine::EntitySystem;

TEST(EntityID, DefaultConstructedIsNull)
{
	EntityID id;

	EXPECT_TRUE(id.IsNull());
	EXPECT_FALSE(bool(id));
}

TEST(EntityID, NullConstantIsNull)
{
	EntityID id = EntityID::Null;

	EXPECT_TRUE(id.IsNull());
	EXPECT_FALSE(bool(id));
}

TEST(EntityID, IndexAndVersionRoundTripThroughConstructor)
{
	EntityID id(42, 7);

	EXPECT_EQ(42, id.Index());
	EXPECT_EQ(7, id.Version());
	EXPECT_FALSE(id.IsNull());
	EXPECT_TRUE(bool(id));
}

TEST(EntityID, ZeroIndexAndVersionIsNotNull)
{
	// Null is encoded as UINT32_MAX, so an all-zero id must not be confused with it.
	EntityID id(0, 0);

	EXPECT_FALSE(id.IsNull());
	EXPECT_EQ(0, id.Index());
	EXPECT_EQ(0, id.Version());
}

TEST(EntityID, ValueRoundTripsThroughRawConstructor)
{
	EntityID original(123, 45);
	EntityID copy(original.Value());

	EXPECT_EQ(original, copy);
	EXPECT_EQ(original.Index(), copy.Index());
	EXPECT_EQ(original.Version(), copy.Version());
}

TEST(EntityID, EqualityComparesIndexAndVersion)
{
	EntityID a(1, 2);
	EntityID sameAsA(1, 2);
	EntityID differentVersion(1, 3);
	EntityID differentIndex(2, 2);

	EXPECT_EQ(a, sameAsA);
	EXPECT_NE(a, differentVersion);
	EXPECT_NE(a, differentIndex);
}

TEST(EntityID, NullIsNotEqualToAnyRealEntity)
{
	EntityID id(0, 0);

	EXPECT_NE(id, EntityID::Null);
}
