#include <EntitySystem.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

namespace {

void ExpectPositionEqual(const float3& actual, const float3& expected)
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
	const auto entity = scene.Create();

	ExpectPositionEqual(scene.GetTransform(entity).Position(), float3(0, 0, 0));
}

TEST(Transform, SetPositionUpdatesPosition)
{
	Scene scene;
	const auto entity = scene.Create();

	scene.GetTransform(entity).SetPosition(float3(1, 2, 3));

	ExpectPositionEqual(scene.GetTransform(entity).Position(), float3(1, 2, 3));
}

TEST(Transform, TranslateAddsToPosition)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetPosition(float3(1, 2, 3));
	transform.Translate(float3(1, 1, 1));

	ExpectPositionEqual(scene.GetTransform(entity).Position(), float3(2, 3, 4));
}

// -- Transform: orientation --------------------------------------------------

TEST(Transform, DefaultOrientationIsZeroEulerAngles)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);

	EXPECT_TRUE(transform.UsingEulerAngles());
	EXPECT_FALSE(transform.UsingQuaternion());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerX().count());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerY().count());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerZ().count());
	EXPECT_EQ(euler_rotation_order::zxy, transform.RotationOrder());
}

TEST(Transform, SetEulerAnglesSwitchesToEulerModeAndRoundTrips)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetEulerAngles(degrees(45), degrees(30), degrees(60), euler_rotation_order::zyx);

	EXPECT_TRUE(transform.UsingEulerAngles());
	EXPECT_FALSE(transform.UsingQuaternion());
	EXPECT_FLOAT_EQ(45.0f, transform.EulerX().count());
	EXPECT_FLOAT_EQ(30.0f, transform.EulerY().count());
	EXPECT_FLOAT_EQ(60.0f, transform.EulerZ().count());
	EXPECT_EQ(euler_rotation_order::zyx, transform.RotationOrder());
}

TEST(Transform, EulerAnglesFallBackToZeroWhileUsingQuaternion)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetQuaternion(quaternion(0.0f, 1.0f, 0.0f, 0.0f));

	EXPECT_TRUE(transform.UsingQuaternion());
	EXPECT_FALSE(transform.UsingEulerAngles());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerX().count());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerY().count());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerZ().count());
	EXPECT_EQ(euler_rotation_order::zxy, transform.RotationOrder());
}

TEST(Transform, SetQuaternionSwitchesToQuaternionModeAndRoundTrips)
{
	Scene scene;
	const auto entity = scene.Create();

	const quaternion q(0.0f, 1.0f, 0.0f, 0.0f);

	auto transform = scene.GetTransform(entity);
	transform.SetQuaternion(q);

	EXPECT_TRUE(transform.UsingQuaternion());
	EXPECT_FLOAT_EQ(q.w, transform.Quaternion().w);
	EXPECT_FLOAT_EQ(q.x, transform.Quaternion().x);
	EXPECT_FLOAT_EQ(q.y, transform.Quaternion().y);
	EXPECT_FLOAT_EQ(q.z, transform.Quaternion().z);
}

TEST(Transform, QuaternionFallsBackToIdentityWhileUsingEulerAngles)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetEulerAngles(degrees(10), degrees(0), degrees(0));

	EXPECT_FLOAT_EQ(quaternion::identity.w, transform.Quaternion().w);
	EXPECT_FLOAT_EQ(quaternion::identity.x, transform.Quaternion().x);
	EXPECT_FLOAT_EQ(quaternion::identity.y, transform.Quaternion().y);
	EXPECT_FLOAT_EQ(quaternion::identity.z, transform.Quaternion().z);
}

TEST(Transform, ResetRestoresDefaultPositionAndOrientation)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetPosition(float3(1, 2, 3));
	transform.SetQuaternion(quaternion(0.0f, 1.0f, 0.0f, 0.0f));

	transform.Reset();

	ExpectPositionEqual(transform.Position(), float3(0, 0, 0));
	EXPECT_TRUE(transform.UsingEulerAngles());
	EXPECT_FLOAT_EQ(0.0f, transform.EulerX().count());
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
	const auto source = scene.Create();
	const auto destination = scene.Create();

	auto sourceTransform = scene.GetTransform(source);
	sourceTransform.SetPosition(float3(1, 2, 3));
	sourceTransform.SetEulerAngles(degrees(10), degrees(20), degrees(30));

	auto destinationTransform = scene.GetTransform(destination);
	sourceTransform.CopyTo(destinationTransform);

	ExpectPositionEqual(destinationTransform.Position(), float3(1, 2, 3));
	EXPECT_FLOAT_EQ(10.0f, destinationTransform.EulerX().count());
	EXPECT_FLOAT_EQ(20.0f, destinationTransform.EulerY().count());
	EXPECT_FLOAT_EQ(30.0f, destinationTransform.EulerZ().count());
}

TEST(CTransform, CopyToCopiesPositionAndOrientationToAnotherEntityInTheSameScene)
{
	Scene scene;
	const auto source = scene.Create();
	const auto destination = scene.Create();

	scene.GetTransform(source).SetPosition(float3(4, 5, 6));
	const CTransform sourceView = scene.GetTransform(source);

	auto destinationTransform = scene.GetTransform(destination);
	sourceView.CopyTo(destinationTransform);

	ExpectPositionEqual(destinationTransform.Position(), float3(4, 5, 6));
}

// -- CTransform ---------------------------------------------------------------

TEST(CTransform, ConstSceneGetTransformReturnsCTransform)
{
	Scene scene;
	const auto entity = scene.Create();
	scene.GetTransform(entity).SetPosition(float3(4, 5, 6));

	const Scene& constScene = scene;
	const CTransform view = constScene.GetTransform(entity);

	ExpectPositionEqual(view.Position(), float3(4, 5, 6));
}

TEST(CTransform, MirrorsUnderlyingTransformState)
{
	Scene scene;
	const auto entity = scene.Create();

	auto transform = scene.GetTransform(entity);
	transform.SetPosition(float3(7, 8, 9));
	transform.SetEulerAngles(degrees(15), degrees(0), degrees(0));

	const CTransform constView(transform);

	ExpectPositionEqual(constView.Position(), float3(7, 8, 9));
	EXPECT_TRUE(constView.UsingEulerAngles());
	EXPECT_FLOAT_EQ(15.0f, constView.EulerX().count());
	EXPECT_EQ(entity, constView.Entity());
}

TEST(Transform, EntityReturnsOwningEntityID)
{
	Scene scene;
	const auto entity = scene.Create();

	EXPECT_EQ(entity, scene.GetTransform(entity).Entity());
}

TEST(CTransform, AssignmentFromTransformUpdatesView)
{
	Scene scene;
	const auto entityA = scene.Create();
	const auto entityB = scene.Create();

	scene.GetTransform(entityA).SetPosition(float3(1, 0, 0));
	scene.GetTransform(entityB).SetPosition(float3(0, 1, 0));

	CTransform view = scene.GetTransform(entityA);
	ExpectPositionEqual(view.Position(), float3(1, 0, 0));

	view = scene.GetTransform(entityB);
	ExpectPositionEqual(view.Position(), float3(0, 1, 0));
}
