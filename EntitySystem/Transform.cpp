#include "Precomp.h"

using namespace ::CainEngine::EntitySystem;

Transform::Transform() noexcept
	: m_scene(nullptr),
	m_entity(EntityID::Null)
{
}

Transform::Transform(Scene* scene, EntityID entity) noexcept
	: m_scene(scene),
	m_entity(entity)
{
}

Transform::~Transform() noexcept
{
}

const float3& Transform::Position() const noexcept
{
	return GetData().position;
}

void Transform::SetPosition(const float3& position)
{
	GetData().position = position;
	m_scene->SetDirty(m_entity);
}

void Transform::Translate(const float3& translation)
{
	GetData().position += translation;
	m_scene->SetDirty(m_entity);
}

bool Transform::UsingEulerAngles() const noexcept
{
	return std::holds_alternative<Scene::EulerAngles>(GetData().orientation);
}

bool Transform::UsingQuaternion() const noexcept
{
	return std::holds_alternative<quaternion>(GetData().orientation);
}

const quaternion& Transform::Quaternion() const noexcept
{
	auto quat = std::get_if<quaternion>(&GetData().orientation);

	if (quat) [[likely]]
		return *quat;

	return quaternion::identity;
}

degrees Transform::EulerX() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->x;

	return degrees::zero;
}

degrees Transform::EulerY() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->y;

	return degrees::zero;
}

degrees Transform::EulerZ() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->z;

	return degrees::zero;
}

euler_rotation_order Transform::RotationOrder() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->order;

	return euler_rotation_order::zxy;
}

void Transform::SetQuaternion(const quaternion& quat)
{
	GetData().orientation = quat;
	m_scene->SetDirty(m_entity);
}

void Transform::SetEulerAngles(degrees x, degrees y, degrees z, euler_rotation_order order)
{
	GetData().orientation = Scene::EulerAngles{ x, y, z, order };
	m_scene->SetDirty(m_entity);
}

void Transform::Reset()
{
	GetData().position = { 0,0,0 };
	GetData().orientation = Scene::EulerAngles{ 0, 0, 0, euler_rotation_order::zxy };
	m_scene->SetDirty(m_entity);
}

void Transform::CopyTo(Transform& other) const
{
	COMMON_ASSERT(m_entity != other.m_entity);
	COMMON_ASSERT(m_scene != other.m_scene);

	const auto& thisData = GetData();
	auto& otherData = other.GetData();

	otherData.position = thisData.position;
	otherData.orientation = thisData.orientation;

	m_scene->SetDirty(other.m_entity);
}

const Scene::EntityData& Transform::GetData() const noexcept
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->IsAlive(m_entity));

	return m_scene->GetFromID(m_entity);
}

Scene::EntityData& Transform::GetData() noexcept
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->IsAlive(m_entity));

	return m_scene->GetFromID(m_entity);
}