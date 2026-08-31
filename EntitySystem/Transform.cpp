#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

template<bool IsConst>
TransformImpl<IsConst>::TransformImpl() noexcept
	: m_scene(nullptr)
	, m_entity(EntityID::Null)
{ }

template<bool IsConst>
TransformImpl<IsConst>::TransformImpl(SceneT* scene, EntityID entity) noexcept
	: m_scene(scene)
	, m_entity(entity)
{ }

template<bool IsConst>
TransformImpl<IsConst>::~TransformImpl()
{ }

template<bool IsConst>
TransformImpl<IsConst>::TransformImpl(const TransformImpl<false>& other) noexcept
	requires(IsConst)
	: m_scene(other.m_scene)
	, m_entity(other.m_entity)
{ }

template<bool IsConst>
TransformImpl<IsConst>& TransformImpl<IsConst>::operator=(
	const TransformImpl<false>& other) noexcept
	requires(IsConst)
{
	m_scene = other.m_scene;
	m_entity = other.m_entity;

	return *this;
}

template<bool IsConst>
const float3& TransformImpl<IsConst>::position() const noexcept
{
	return getData().position;
}

template<bool IsConst>
void TransformImpl<IsConst>::setPosition(const float3& position)
	requires(!IsConst)
{
	getData().position = position;
	m_scene->setDirty(m_entity);
}

template<bool IsConst>
void TransformImpl<IsConst>::translate(const float3& translation)
	requires(!IsConst)
{
	getData().position += translation;
	m_scene->setDirty(m_entity);
}

template<bool IsConst>
bool TransformImpl<IsConst>::usingEulerAngles() const noexcept
{
	return std::holds_alternative<Scene::EulerAngles>(getData().orientation);
}

template<bool IsConst>
bool TransformImpl<IsConst>::usingQuaternion() const noexcept
{
	return std::holds_alternative<quaternion>(getData().orientation);
}

template<bool IsConst>
const quaternion& TransformImpl<IsConst>::rotation() const noexcept
{
	auto quat = std::get_if<quaternion>(&getData().orientation);

	if(quat) [[likely]]
		return *quat;

	return quaternion::identity;
}

template<bool IsConst>
degrees TransformImpl<IsConst>::eulerX() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&getData().orientation);

	if(euler) [[likely]]
		return euler->x;

	return degrees::zero;
}

template<bool IsConst>
degrees TransformImpl<IsConst>::eulerY() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&getData().orientation);

	if(euler) [[likely]]
		return euler->y;

	return degrees::zero;
}

template<bool IsConst>
degrees TransformImpl<IsConst>::eulerZ() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&getData().orientation);

	if(euler) [[likely]]
		return euler->z;

	return degrees::zero;
}

template<bool IsConst>
euler_rotation_order TransformImpl<IsConst>::rotationOrder() const noexcept
{
	auto euler = std::get_if<Scene::EulerAngles>(&getData().orientation);

	if(euler) [[likely]]
		return euler->order;

	return euler_rotation_order::zxy;
}

template<bool IsConst>
void TransformImpl<IsConst>::setQuaternion(const quaternion& quat)
	requires(!IsConst)
{
	getData().orientation = quat;
	m_scene->setDirty(m_entity);
}

template<bool IsConst>
void TransformImpl<IsConst>::setEulerAngles(
	degrees x, degrees y, degrees z, euler_rotation_order order)
	requires(!IsConst)
{
	getData().orientation = Scene::EulerAngles{ x, y, z, order };
	m_scene->setDirty(m_entity);
}

template<bool IsConst>
void TransformImpl<IsConst>::reset()
	requires(!IsConst)
{
	getData().position = { 0, 0, 0 };
	getData().orientation = Scene::EulerAngles{ 0, 0, 0, euler_rotation_order::zxy };
	m_scene->setDirty(m_entity);
}

template<bool IsConst>
EntityID TransformImpl<IsConst>::entity() const noexcept
{
	return m_entity;
}

template<bool IsConst>
void TransformImpl<IsConst>::copyTo(TransformImpl<false>& other) const
{
	COMMON_ASSERT(m_entity != other.m_entity);
	COMMON_ASSERT(m_scene == other.m_scene);

	const auto& thisData = getData();
	auto& otherData = other.getData();

	otherData.position = thisData.position;
	otherData.orientation = thisData.orientation;

	other.m_scene->setDirty(other.m_entity);
}

template<bool IsConst>
const Scene::EntityData& TransformImpl<IsConst>::getData() const noexcept
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->isAlive(m_entity));

	return m_scene->getFromId(m_entity);
}

template<bool IsConst>
Scene::EntityData& TransformImpl<IsConst>::getData() noexcept
	requires(!IsConst)
{
	COMMON_ASSERT(m_scene);
	COMMON_ASSERT(m_scene->isAlive(m_entity));

	return m_scene->getFromId(m_entity);
}

template class TransformImpl<false>;
template class TransformImpl<true>;
