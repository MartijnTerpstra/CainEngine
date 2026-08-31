#include <EntitySystem.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

namespace {

// matrix3x4 has no operator==, so matrices are compared row-by-row instead.
//
// NOTE: this deliberately uses operator[] rather than the (equivalent, per
// EntitySystem/Dependencies/mst/mst/mx_math_matrix.h) get_row(): get_row()
// crashes for matrix<float, 3, 4> in this vendored build of mst (a
// third-party dependency, not CainEngine code), while operator[] returns the
// same row data without crashing.
void expectMatricesEqual(const matrix3x4& actual, const matrix3x4& expected)
{
	for(size_t row = 0; row < 4; ++row)
	{
		const auto& a = actual[row];
		const auto& e = expected[row];

		EXPECT_FLOAT_EQ(e.x, a.x) << "row " << row;
		EXPECT_FLOAT_EQ(e.y, a.y) << "row " << row;
		EXPECT_FLOAT_EQ(e.z, a.z) << "row " << row;
	}
}

} // namespace

// -- Entity lifecycle --------------------------------------------------------

TEST(Scene, CreateReturnsAliveUnnamedEntity)
{
	Scene scene;

	const auto entity = scene.create();

	EXPECT_TRUE(scene.isAlive(entity));
	EXPECT_TRUE(scene.getName(entity).empty());
	EXPECT_EQ(1u, scene.liveEntities());
	EXPECT_EQ(0u, scene.deadEntities());
}

TEST(Scene, CreateWithNameSetsName)
{
	Scene scene;

	const auto entity = scene.create("Player");

	EXPECT_EQ("Player", scene.getName(entity));
	EXPECT_EQ(entity, scene.find(std::string_view("Player")));
}

TEST(Scene, FindByNameReturnsNullWhenMissing)
{
	Scene scene;

	scene.create("Player");

	EXPECT_EQ(EntityID::Null, scene.find(std::string_view("NoSuchEntity")));
}

TEST(Scene, DestroyMakesEntityNotAlive)
{
	Scene scene;

	const auto entity = scene.create();

	scene.destroy(entity);

	EXPECT_FALSE(scene.isAlive(entity));
	EXPECT_EQ(0u, scene.liveEntities());
	EXPECT_EQ(1u, scene.deadEntities());
}

TEST(Scene, DestroyedNameIsNoLongerFindable)
{
	Scene scene;

	const auto entity = scene.create("Player");

	scene.destroy(entity);

	EXPECT_EQ(EntityID::Null, scene.find(std::string_view("Player")));
}

TEST(Scene, RecreatingAfterDestroyReusesIndexWithNewVersion)
{
	Scene scene;

	const auto first = scene.create();
	scene.destroy(first);
	const auto second = scene.create();

	EXPECT_EQ(first.index(), second.index());
	EXPECT_EQ(first.version() + 1, second.version());
	EXPECT_FALSE(scene.isAlive(first));
	EXPECT_TRUE(scene.isAlive(second));
}

TEST(Scene, MultipleEntitiesGetDistinctIndices)
{
	Scene scene;

	const auto a = scene.create();
	const auto b = scene.create();
	const auto c = scene.create();

	EXPECT_NE(a.index(), b.index());
	EXPECT_NE(b.index(), c.index());
	EXPECT_NE(a.index(), c.index());
	EXPECT_EQ(3u, scene.liveEntities());
}

TEST(Scene, ClearRemovesAllEntitiesAndNames)
{
	Scene scene;

	scene.create("A");
	scene.create("B");
	scene.create();

	scene.clear();

	EXPECT_EQ(0u, scene.liveEntities());
	EXPECT_EQ(EntityID::Null, scene.find(std::string_view("A")));
	EXPECT_EQ(EntityID::Null, scene.find(std::string_view("B")));
}

TEST(Scene, ClearOnEmptySceneIsSafe)
{
	Scene scene;

	scene.clear();

	EXPECT_EQ(0u, scene.liveEntities());
}

TEST(Scene, CapacityIsZeroForFreshScene)
{
	Scene scene;

	EXPECT_EQ(0u, scene.capacity());
}

TEST(Scene, CapacityGrowsAfterCreatingAnEntity)
{
	Scene scene;

	scene.create();

	EXPECT_GE(scene.capacity(), 1u);
}

// -- Unique IDs ---------------------------------------------------------------

TEST(Scene, SetAndGetUniqueID)
{
	Scene scene;

	const auto entity = scene.create();
	const auto id = uuid::create();

	scene.setUniqueId(entity, id);

	ASSERT_NE(nullptr, scene.getUniqueId(entity));
	EXPECT_EQ(id, *scene.getUniqueId(entity));
	EXPECT_EQ(entity, scene.find(id));
}

TEST(Scene, EntityWithoutUniqueIDHasNullptr)
{
	Scene scene;

	const auto entity = scene.create();

	EXPECT_EQ(nullptr, scene.getUniqueId(entity));
}

TEST(Scene, FindByUuidReturnsNullWhenMissing)
{
	Scene scene;

	EXPECT_EQ(EntityID::Null, scene.find(uuid::create()));
}

// -- Callbacks ------------------------------------------------------------

TEST(Scene, CreateCallbackIsInvokedOnCreate)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;
	EntityID createdEntity;

	scene.addCreateCallback(&callbackOwner, [&](Scene& s, EntityID e) {
		++invocationCount;
		createdEntity = e;
	});

	const auto entity = scene.create();

	EXPECT_EQ(1, invocationCount);
	EXPECT_EQ(entity, createdEntity);
}

TEST(Scene, DestroyCallbackIsInvokedOnDestroy)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;
	EntityID destroyedEntity;

	scene.addDestroyCallback(&callbackOwner, [&](Scene& s, EntityID e) {
		++invocationCount;
		destroyedEntity = e;
	});

	const auto entity = scene.create();
	scene.destroy(entity);

	EXPECT_EQ(1, invocationCount);
	EXPECT_EQ(entity, destroyedEntity);
}

TEST(Scene, RemoveAllCallbacksStopsFurtherInvocations)
{
	Scene scene;
	int callbackOwner = 0;
	int invocationCount = 0;

	scene.addCreateCallback(&callbackOwner, [&](Scene&, EntityID) { ++invocationCount; });
	scene.removeAllCallbacks(&callbackOwner);

	scene.create();

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

	const auto entity = scene.create();

	EXPECT_EQ(EntityID::Null, scene.getParent(entity));
	EXPECT_TRUE(scene.getChildren(entity).empty());
}

TEST(Scene, ClearingParentOfUnparentedEntityIsSafe)
{
	Scene scene;

	const auto entity = scene.create();

	scene.setParent(entity, EntityID::Null);

	EXPECT_EQ(EntityID::Null, scene.getParent(entity));
}

TEST(Scene, SetParentEstablishesParentChildRelationship)
{
	Scene scene;
	const auto parent = scene.create();
	const auto child = scene.create();

	scene.setParent(child, parent);

	EXPECT_EQ(parent, scene.getParent(child));
	ASSERT_EQ(1u, scene.getChildren(parent).size());
	EXPECT_EQ(child, scene.getChildren(parent)[0]);
}

TEST(Scene, SetParentToNullClearsParent)
{
	Scene scene;
	const auto parent = scene.create();
	const auto child = scene.create();

	scene.setParent(child, parent);
	scene.setParent(child, EntityID::Null);

	EXPECT_EQ(EntityID::Null, scene.getParent(child));
	EXPECT_TRUE(scene.getChildren(parent).empty());
}

// -- Matrices ---------------------------------------------------------------

TEST(Scene, GlobalMatrixIsIdentityForFreshEntity)
{
	Scene scene;

	const auto entity = scene.create();

	expectMatricesEqual(scene.getGlobalMatrix(entity), matrix3x4::identity);
}

TEST(Scene, BuildMatricesAppliesPositionForRootEntity)
{
	Scene scene;

	const auto entity = scene.create();
	const float3 position(1, 2, 3);

	scene.getTransform(entity).setPosition(position);
	scene.buildMatrices();

	// No parent and the default (zero) orientation, so the global matrix should
	// match the same position-only local matrix Scene::BuildMatrices computes.
	const matrix3x4 expected(position);

	expectMatricesEqual(scene.getGlobalMatrix(entity), expected);
}
