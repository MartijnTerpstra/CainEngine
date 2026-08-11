#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

template <bool IsConst>
TransformImpl<IsConst>::TransformImpl() noexcept
	: m_scene(nullptr),
	m_entity(EntityID::Null)
{
}

template <bool IsConst>
TransformImpl<IsConst>::TransformImpl(SceneT* scene, EntityID entity) noexcept
	: m_scene(scene),
	m_entity(entity)
{
}

template <bool IsConst>
TransformImpl<IsConst>::~TransformImpl()
{
}

template <bool IsConst>
TransformImpl<IsConst>::TransformImpl(const TransformImpl<false>& other) noexcept requires IsConst
	: m_scene(other.m_scene),
	m_entity(other.m_entity)
{
}

template <bool IsConst>
TransformImpl<IsConst>& TransformImpl<IsConst>::operator = (const TransformImpl<false>& other) noexcept requires IsConst
{
	m_scene = other.m_scene;
	m_entity = other.m_entity;

	return *this;
}

template <bool IsConst>
const float3& TransformImpl<IsConst>::Position() const noexcept
{
	return GetData().position;
}

template <bool IsConst>
void TransformImpl<IsConst>::SetPosition(const float3& position) requires (!IsConst)
{
	GetData().position = position;
	m_scene->SetDirty(m_entity);
}

template <bool IsConst>
void TransformImpl<IsConst>::Translate(const float3& translation) requires (!IsConst)
{
	GetData().position += translation;
	m_scene->SetDirty(m_entity);
}

template <bool IsConst>
bool TransformImpl<IsConst>::UsingEulerAngles() const noexcept
{
	return std::holds_alternative<Scene::EulerAngles>(GetData().orientation);
}

template <bool IsConst>
bool TransformImpl<IsConst>::UsingQuaternion() const noexcept
{
	return std::holds_alternative<quaternion>(GetData().orientation);
}

template <bool IsConst>
const quaternion& TransformImpl<IsConst>::Quaternion() const noexcept
{
	auto quat = std::get_if<quaternion>(&GetData().orientation);

	if (quat) [[likely]]
		return *quat;

	return quaternion::identity;
}

template <bool IsConst>
degrees TransformImpl<IsConst>::EulerX() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->x;

	return degrees::zero;
}

template <bool IsConst>
degrees TransformImpl<IsConst>::EulerY() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->y;

	return degrees::zero;
}

template <bool IsConst>
degrees TransformImpl<IsConst>::EulerZ() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->z;

	return degrees::zero;
}

template <bool IsConst>
euler_rotation_order TransformImpl<IsConst>::RotationOrder() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&GetData().orientation);

	if (euler) [[likely]]
		return euler->order;

	return euler_rotation_order::zxy;
}

template <bool IsConst>
void TransformImpl<IsConst>::SetQuaternion(const quaternion& quat) requires (!IsConst)
{
	GetData().orientation = quat;
	m_scene->SetDirty(m_entity);
}

template <bool IsConst>
void TransformImpl<IsConst>::SetEulerAngles(degrees x, degrees y, degrees z, euler_rotation_order order) requires (!IsConst)
{
	GetData().orientation = Scene::EulerAngles{ x, y, z, order };
	m_scene->SetDirty(m_entity);
}

template <bool IsConst>
void TransformImpl<IsConst>::Reset() requires (!IsConst)
{
	GetData().position = { 0,0,0 };
	GetData().orientation = Scene::EulerAngles{ 0, 0, 0, euler_rotation_order::zxy };
	m_scene->SetDirty(m_entity);
}

template <bool IsConst>
EntityID TransformImpl<IsConst>::Entity() const noexcept
{
	return m_entity;
}

template <bool IsConst>
void TransformImpl<IsConst>::CopyTo(TransformImpl<false>& other) const
{
	COMMON_ASSERT(m_entity != other.m_entity);
	COMMON_ASSERT(m_scene == other.m_scene);

	const auto& thisData = GetData();
	auto& otherData = other.GetData();

	otherData.position = thisData.position;
	otherData.orientation = thisData.orientation;

	other.m_scene->SetDirty(other.m_entity);
}

template <bool IsConst>
const Scene::EntityData& TransformImpl<IsConst>::GetData() const noexcept
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->IsAlive(m_entity));

	return m_scene->GetFromID(m_entity);
}

template <bool IsConst>
Scene::EntityData& TransformImpl<IsConst>::GetData() noexcept requires (!IsConst)
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->IsAlive(m_entity));

	return m_scene->GetFromID(m_entity);
}

template class TransformImpl<false>;
template class TransformImpl<true>;
