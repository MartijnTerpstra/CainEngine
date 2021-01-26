#include "Precomp.h"

using namespace ::CainEngine::EntitySystem;

CTransform::CTransform() noexcept
	: m_scene(nullptr),
	m_entity(EntityID::Null)
{
}

CTransform::CTransform(const Scene* scene, EntityID entity) noexcept
	: m_scene(scene),
	m_entity(entity)
{
}

CTransform::CTransform(const Transform& other) noexcept
	: m_scene(other.m_scene),
	m_entity(other.m_entity)
{
}

CTransform& CTransform::operator = (const Transform& other) noexcept
{
	m_scene = other.m_scene;
	m_entity = other.m_entity;

	return *this;
}

CTransform::~CTransform()
{
}

const float3& CTransform::Position() const noexcept
{
	return GetData().position;
}

bool CTransform::UsingEulerAngles() const noexcept
{
	return std::holds_alternative<Scene::EulerAngles>(GetData().orientation);
}

bool CTransform::UsingQuaternion() const noexcept
{
	return std::holds_alternative<quaternion>(GetData().orientation);
}

const quaternion& CTransform::Quaternion() const noexcept
{
	auto quat = std::get_if<quaternion>(&GetData().orientation);

	if (quat)
		return *quat;

	return quaternion::identity;
}

degrees CTransform::EulerX() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler)
		return euler->x;

	return degrees::zero;
}

degrees CTransform::EulerY() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler)
		return euler->y;

	return degrees::zero;
}

degrees CTransform::EulerZ() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler)
		return euler->z;

	return degrees::zero;
}

euler_rotation_order CTransform::RotationOrder() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler)
		return euler->order;

	return euler_rotation_order::zxy;
}

void CTransform::CopyTo(Transform& other) const
{
	COMMON_ASSERT(m_entity != other.m_entity);
	COMMON_ASSERT(m_scene != other.m_scene);

	const auto& thisData = GetData();
	auto& otherData = other.GetData();

	otherData.position = thisData.position;
	otherData.orientation = thisData.orientation;

	other.m_scene->SetDirty(other.m_entity);
}

const Scene::EntityData& CTransform::GetData() const noexcept
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->IsAlive(m_entity));

	return m_scene->GetFromID(m_entity);
}