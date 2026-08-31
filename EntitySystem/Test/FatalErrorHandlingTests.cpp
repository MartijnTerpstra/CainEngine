#include <EntitySystem.h>
#include <CommonTestSupport.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;
using namespace ::CainEngine::TestSupport;

// Proves CommonTestSupport's FatalErrorTest works for EntitySystem too, not
// just for the Common library it's defined in - these exercise real
// COMMON_ASSERT()s inside Scene::SetParent() rather than a synthetic one.

TEST_F(FatalErrorTest, SetParentToSelfTriggersAnAssert)
{
	Scene scene;
	const auto entity = scene.create();

	EXPECT_THROW(scene.setParent(entity, entity), FatalErrorTriggered);
}

TEST_F(FatalErrorTest, SetParentToADeadEntityTriggersAnAssert)
{
	Scene scene;
	const auto entity = scene.create();
	const auto deadParent = scene.create();
	scene.destroy(deadParent);

	EXPECT_THROW(scene.setParent(entity, deadParent), FatalErrorTriggered);
}
