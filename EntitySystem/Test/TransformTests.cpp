#include <EntitySystem.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

namespace {

void expectPositionEqual(const float3& actual, const float3& expected)
{
	EXPECT_FLOAT_EQ(expected.x, actual.x);
	EXPECT_FLOAT_EQ(expected.y, actual.y);
	EXPECT_FLOAT_EQ(expected.z, actual.z);
}

} // namespace

// -- Transform: position -----------------------------------------------------

TEST(Transform, DefaultPositionIsZero)
{
	Scene scene;
	const auto entity = scene.create();

	expectPositionEqual(scene.getTransform(entity).position(), float3(0, 0, 0));
}

TEST(Transform, SetPositionUpdatesPosition)
{
	Scene scene;
	const auto entity = scene.create();

	scene.getTransform(entity).setPosition(float3(1, 2, 3));

	expectPositionEqual(scene.getTransform(entity).position(), float3(1, 2, 3));
}

TEST(Transform, TranslateAddsToPosition)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setPosition(float3(1, 2, 3));
	transform.translate(float3(1, 1, 1));

	expectPositionEqual(scene.getTransform(entity).position(), float3(2, 3, 4));
}

// -- Transform: orientation --------------------------------------------------

TEST(Transform, DefaultOrientationIsZeroEulerAngles)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);

	EXPECT_TRUE(transform.usingEulerAngles());
	EXPECT_FALSE(transform.usingQuaternion());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerX().count());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerY().count());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerZ().count());
	EXPECT_EQ(euler_rotation_order::zxy, transform.rotationOrder());
}

TEST(Transform, SetEulerAnglesSwitchesToEulerModeAndRoundTrips)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setEulerAngles(degrees(45), degrees(30), degrees(60), euler_rotation_order::zyx);

	EXPECT_TRUE(transform.usingEulerAngles());
	EXPECT_FALSE(transform.usingQuaternion());
	EXPECT_FLOAT_EQ(45.0f, transform.eulerX().count());
	EXPECT_FLOAT_EQ(30.0f, transform.eulerY().count());
	EXPECT_FLOAT_EQ(60.0f, transform.eulerZ().count());
	EXPECT_EQ(euler_rotation_order::zyx, transform.rotationOrder());
}

TEST(Transform, EulerAnglesFallBackToZeroWhileUsingQuaternion)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setQuaternion(quaternion(0.0f, 1.0f, 0.0f, 0.0f));

	EXPECT_TRUE(transform.usingQuaternion());
	EXPECT_FALSE(transform.usingEulerAngles());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerX().count());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerY().count());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerZ().count());
	EXPECT_EQ(euler_rotation_order::zxy, transform.rotationOrder());
}

TEST(Transform, SetQuaternionSwitchesToQuaternionModeAndRoundTrips)
{
	Scene scene;
	const auto entity = scene.create();

	const quaternion q(0.0f, 1.0f, 0.0f, 0.0f);

	auto transform = scene.getTransform(entity);
	transform.setQuaternion(q);

	EXPECT_TRUE(transform.usingQuaternion());
	EXPECT_FLOAT_EQ(q.w, transform.rotation().w);
	EXPECT_FLOAT_EQ(q.x, transform.rotation().x);
	EXPECT_FLOAT_EQ(q.y, transform.rotation().y);
	EXPECT_FLOAT_EQ(q.z, transform.rotation().z);
}

TEST(Transform, QuaternionFallsBackToIdentityWhileUsingEulerAngles)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setEulerAngles(degrees(10), degrees(0), degrees(0));

	EXPECT_FLOAT_EQ(quaternion::identity.w, transform.rotation().w);
	EXPECT_FLOAT_EQ(quaternion::identity.x, transform.rotation().x);
	EXPECT_FLOAT_EQ(quaternion::identity.y, transform.rotation().y);
	EXPECT_FLOAT_EQ(quaternion::identity.z, transform.rotation().z);
}

TEST(Transform, ResetRestoresDefaultPositionAndOrientation)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setPosition(float3(1, 2, 3));
	transform.setQuaternion(quaternion(0.0f, 1.0f, 0.0f, 0.0f));

	transform.reset();

	expectPositionEqual(transform.position(), float3(0, 0, 0));
	EXPECT_TRUE(transform.usingEulerAngles());
	EXPECT_FLOAT_EQ(0.0f, transform.eulerX().count());
}

// -- Transform::CopyTo / CTransform::CopyTo ----------------------------------
//
// Both Transform::CopyTo and CTransform::CopyTo used to have a bug mirroring
// Scene::SetParent's (see bugs.md #4): their assert required
// `m_scene != other.m_scene`, rejecting the only sane use case (copying
// between two entities of the *same* scene), and Transform::CopyTo dirtied
// the wrong scene besides. Now fixed, so real functional coverage lives here.

TEST(Transform, CopyToCopiesPositionAndOrientationToAnotherEntityInTheSameScene)
{
	Scene scene;
	const auto source = scene.create();
	const auto destination = scene.create();

	auto sourceTransform = scene.getTransform(source);
	sourceTransform.setPosition(float3(1, 2, 3));
	sourceTransform.setEulerAngles(degrees(10), degrees(20), degrees(30));

	auto destinationTransform = scene.getTransform(destination);
	sourceTransform.copyTo(destinationTransform);

	expectPositionEqual(destinationTransform.position(), float3(1, 2, 3));
	EXPECT_FLOAT_EQ(10.0f, destinationTransform.eulerX().count());
	EXPECT_FLOAT_EQ(20.0f, destinationTransform.eulerY().count());
	EXPECT_FLOAT_EQ(30.0f, destinationTransform.eulerZ().count());
}

TEST(CTransform, CopyToCopiesPositionAndOrientationToAnotherEntityInTheSameScene)
{
	Scene scene;
	const auto source = scene.create();
	const auto destination = scene.create();

	scene.getTransform(source).setPosition(float3(4, 5, 6));
	const CTransform sourceView = scene.getTransform(source);

	auto destinationTransform = scene.getTransform(destination);
	sourceView.copyTo(destinationTransform);

	expectPositionEqual(destinationTransform.position(), float3(4, 5, 6));
}

// -- CTransform ---------------------------------------------------------------

TEST(CTransform, ConstSceneGetTransformReturnsCTransform)
{
	Scene scene;
	const auto entity = scene.create();
	scene.getTransform(entity).setPosition(float3(4, 5, 6));

	const Scene& constScene = scene;
	const CTransform view = constScene.getTransform(entity);

	expectPositionEqual(view.position(), float3(4, 5, 6));
}

TEST(CTransform, MirrorsUnderlyingTransformState)
{
	Scene scene;
	const auto entity = scene.create();

	auto transform = scene.getTransform(entity);
	transform.setPosition(float3(7, 8, 9));
	transform.setEulerAngles(degrees(15), degrees(0), degrees(0));

	const CTransform constView(transform);

	expectPositionEqual(constView.position(), float3(7, 8, 9));
	EXPECT_TRUE(constView.usingEulerAngles());
	EXPECT_FLOAT_EQ(15.0f, constView.eulerX().count());
	EXPECT_EQ(entity, constView.entity());
}

TEST(Transform, EntityReturnsOwningEntityID)
{
	Scene scene;
	const auto entity = scene.create();

	EXPECT_EQ(entity, scene.getTransform(entity).entity());
}

TEST(CTransform, AssignmentFromTransformUpdatesView)
{
	Scene scene;
	const auto entityA = scene.create();
	const auto entityB = scene.create();

	scene.getTransform(entityA).setPosition(float3(1, 0, 0));
	scene.getTransform(entityB).setPosition(float3(0, 1, 0));

	CTransform view = scene.getTransform(entityA);
	expectPositionEqual(view.position(), float3(1, 0, 0));

	view = scene.getTransform(entityB);
	expectPositionEqual(view.position(), float3(0, 1, 0));
}
