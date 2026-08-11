#include <EntitySystem.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

namespace {

// matrix4x3 has no operator==, so matrices are compared row-by-row instead.
//
// NOTE: this deliberately uses operator[] rather than the (equivalent, per
// EntitySystem/Dependencies/mst/mst/mx_math_matrix.h) get_row(): get_row()
// crashes for matrix<float, 4, 3> in this vendored build of mst (a
// third-party dependency, not CainEngine code), while operator[] returns the
// same row data without crashing.
void ExpectMatricesEqual(const matrix4x3& actual, const matrix4x3& expected)
{
	for(size_t row = 0; row < 3; ++row)
	{
		const auto& a = actual[row];
		const auto& e = expected[row];

		EXPECT_FLOAT_EQ(e.x, a.x) << "row " << row;
		EXPECT_FLOAT_EQ(e.y, a.y) << "row " << row;
		EXPECT_FLOAT_EQ(e.z, a.z) << "row " << row;
		EXPECT_FLOAT_EQ(e.w, a.w) << "row " << row;
	}
}

} // namespace

// -- Entity lifecycle --------------------------------------------------------

TEST(Scene, CreateReturnsAliveUnnamedEntity)
{
	Scene scene;

	const auto entity = scene.Create();

	EXPECT_TRUE(scene.IsAlive(entity));
	EXPECT_TRUE(scene.GetName(entity).empty());
	EXPECT_EQ(1u, scene.LiveEntities());
	EXPECT_EQ(0u, scene.DeadEntities());
}

TEST(Scene, CreateWithNameSetsName)
{
	Scene scene;

	const auto entity = scene.Create("Player");

	EXPECT_EQ("Player", scene.GetName(entity));
	EXPECT_EQ(entity, scene.Find(std::string_view("Player")));
}

TEST(Scene, FindByNameReturnsNullWhenMissing)
{
	Scene scene;

	scene.Create("Player");

	EXPECT_EQ(EntityID::Null, scene.Find(std::string_view("NoSuchEntity")));
}

TEST(Scene, DestroyMakesEntityNotAlive)
{
	Scene scene;

	const auto entity = scene.Create();

	scene.Destroy(entity);

	EXPECT_FALSE(scene.IsAlive(entity));
	EXPECT_EQ(0u, scene.LiveEntities());
	EXPECT_EQ(1u, scene.DeadEntities());
}

TEST(Scene, DestroyedNameIsNoLongerFindable)
{
	Scene scene;

	const auto entity = scene.Create("Player");

	scene.Destroy(entity);

	EXPECT_EQ(EntityID::Null, scene.Find(std::string_view("Player")));
}

TEST(Scene, RecreatingAfterDestroyReusesIndexWithNewVersion)
{
	Scene scene;

	const auto first = scene.Create();
	scene.Destroy(first);
	const auto second = scene.Create();

	EXPECT_EQ(first.Index(), second.Index());
	EXPECT_EQ(first.Version() + 1, second.Version());
	EXPECT_FALSE(scene.IsAlive(first));
	EXPECT_TRUE(scene.IsAlive(second));
}

TEST(Scene, MultipleEntitiesGetDistinctIndices)
{
	Scene scene;

	const auto a = scene.Create();
	const auto b = scene.Create();
	const auto c = scene.Create();

	EXPECT_NE(a.Index(), b.Index());
	EXPECT_NE(b.Index(), c.Index());
	EXPECT_NE(a.Index(), c.Index());
	EXPECT_EQ(3u, scene.LiveEntities());
}

TEST(Scene, ClearRemovesAllEntitiesAndNames)
{
	Scene scene;

	scene.Create("A");
	scene.Create("B");
	scene.Create();

	scene.Clear();

	EXPECT_EQ(0u, scene.LiveEntities());
	EXPECT_EQ(EntityID::Null, scene.Find(std::string_view("A")));
	EXPECT_EQ(EntityID::Null, scene.Find(std::string_view("B")));
}

TEST(Scene, ClearOnEmptySceneIsSafe)
{
	Scene scene;

	scene.Clear();

	EXPECT_EQ(0u, scene.LiveEntities());
}

TEST(Scene, CapacityIsZeroForFreshScene)
{
	Scene scene;

	EXPECT_EQ(0u, scene.Capacity());
}

TEST(Scene, CapacityGrowsAfterCreatingAnEntity)
{
	Scene scene;

	scene.Create();

	EXPECT_GE(scene.Capacity(), 1u);
}

// -- Unique IDs ---------------------------------------------------------------

TEST(Scene, SetAndGetUniqueID)
{
	Scene scene;

	const auto entity = scene.Create();
	const auto id = uuid::create();

	scene.SetUniqueID(entity, id);

	ASSERT_NE(nullptr, scene.GetUniqueID(entity));
	EXPECT_EQ(id, *scene.GetUniqueID(entity));
	EXPECT_EQ(entity, scene.Find(id));
}

TEST(Scene, EntityWithoutUniqueIDHasNullptr)
{
	Scene scene;

	const auto entity = scene.Create();

	EXPECT_EQ(nullptr, scene.GetUniqueID(entity));
}

TEST(Scene, FindByUuidReturnsNullWhenMissing)
{
	Scene scene;

	EXPECT_EQ(EntityID::Null, scene.Find(uuid::create()));
}

// -- Callbacks ------------------------------------------------------------

TEST(Scene, CreateCallbackIsInvokedOnCreate)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;
	EntityID createdEntity;

	scene.AddCreateCallback(&callbackOwner, [&](Scene& s, EntityID e) {
		++invocationCount;
		createdEntity = e;
	});

	const auto entity = scene.Create();

	EXPECT_EQ(1, invocationCount);
	EXPECT_EQ(entity, createdEntity);
}

TEST(Scene, DestroyCallbackIsInvokedOnDestroy)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;
	EntityID destroyedEntity;

	scene.AddDestroyCallback(&callbackOwner, [&](Scene& s, EntityID e) {
		++invocationCount;
		destroyedEntity = e;
	});

	const auto entity = scene.Create();
	scene.Destroy(entity);

	EXPECT_EQ(1, invocationCount);
	EXPECT_EQ(entity, destroyedEntity);
}

TEST(Scene, RemoveAllCallbacksStopsFurtherInvocations)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;

	scene.AddCreateCallback(&callbackOwner, [&](Scene&, EntityID) { ++invocationCount; });
	scene.RemoveAllCallbacks(&callbackOwner);

	scene.Create();

	EXPECT_EQ(0, invocationCount);
}

// -- Hierarchy ------------------------------------------------------------
//
// Scene::SetParent's cycle-prevention assert used to be inverted (see
// bugs.md #3) which made any ordinary first-time parenting call fail its
// assert. Now fixed, so real SetParent/GetChildren coverage lives here.

TEST(Scene, EntityHasNoParentByDefault)
{
	Scene scene;

	const auto entity = scene.Create();

	EXPECT_EQ(EntityID::Null, scene.GetParent(entity));
	EXPECT_TRUE(scene.GetChildren(entity).empty());
}

TEST(Scene, ClearingParentOfUnparentedEntityIsSafe)
{
	Scene scene;

	const auto entity = scene.Create();

	scene.SetParent(entity, EntityID::Null);

	EXPECT_EQ(EntityID::Null, scene.GetParent(entity));
}

TEST(Scene, SetParentEstablishesParentChildRelationship)
{
	Scene scene;
	const auto parent = scene.Create();
	const auto child = scene.Create();

	scene.SetParent(child, parent);

	EXPECT_EQ(parent, scene.GetParent(child));
	ASSERT_EQ(1u, scene.GetChildren(parent).size());
	EXPECT_EQ(child, scene.GetChildren(parent)[0]);
}

TEST(Scene, SetParentToNullClearsParent)
{
	Scene scene;
	const auto parent = scene.Create();
	const auto child = scene.Create();

	scene.SetParent(child, parent);
	scene.SetParent(child, EntityID::Null);

	EXPECT_EQ(EntityID::Null, scene.GetParent(child));
	EXPECT_TRUE(scene.GetChildren(parent).empty());
}

// -- Matrices ---------------------------------------------------------------

TEST(Scene, GlobalMatrixIsIdentityForFreshEntity)
{
	Scene scene;

	const auto entity = scene.Create();

	ExpectMatricesEqual(scene.GetGlobalMatrix(entity), matrix4x3::identity);
}

TEST(Scene, BuildMatricesAppliesPositionForRootEntity)
{
	Scene scene;

	const auto entity = scene.Create();
	const float3 position(1, 2, 3);

	scene.GetTransform(entity).SetPosition(position);
	scene.BuildMatrices();

	// No parent and the default (zero) orientation, so the global matrix should
	// match the same position-only local matrix Scene::BuildMatrices computes.
	const matrix4x3 expected(position);

	ExpectMatricesEqual(scene.GetGlobalMatrix(entity), expected);
}
