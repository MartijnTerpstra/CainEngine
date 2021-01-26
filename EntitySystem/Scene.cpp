#include "Precomp.h"

#include <cstdlib>

using namespace ::CainEngine::EntitySystem;

Scene::Scene()
{
}

Scene::~Scene()
{
}

EntityID Scene::Create()
{
	int32_t newId = RemoveFromFreeList();
	if (newId == -1)
	{
		COMMON_ASSERT(m_entityCount < 0xFFFFF - 1);

		newId = CreateNew();
	}

	auto& e = m_entities[newId];
	e.parent = EntityID::Null;
	e.position = { 0,0,0 };
	e.orientation = EulerAngles{ 0, 0, 0, euler_rotation_order::zxy };
	e.globalMatrix = matrix4x3::identity;
	e.hasDirtyTransform = false;
	e.name = {};
	e.freeListPrev = -1;
	e.freeListNext = -1;
	e.uniqueId = nullptr;

	return EntityID{ newId, e.version };
}

EntityID Scene::Create(string name)
{
	const auto id = Create();
	SetName(id, move(name));

	return id;
}

EntityID Scene::Find(string_view name) const noexcept
{
	const auto iter = m_names.find(name);

	if (iter == m_names.end())
		return EntityID::Null;

	return iter->second;
}

EntityID Scene::Find(const uuid& id) const noexcept
{
	const auto iter = m_uniqueIds.find(id);

	if (iter == m_uniqueIds.end())
		return EntityID::Null;

	return iter->second;
}

void Scene::Destroy(EntityID entity, bool recursive)
{
	COMMON_ASSERT(IsAlive(entity));

	for (auto& handler : m_destroyHandlers)
	{
		handler.second(*this, entity);
	}

	--m_liveEntityCount;

	ResetParentForDestroy(entity);

	const auto index = entity.Index();

	DestroyAndAddToFreeList(index);

	if (recursive)
	{
		DestroyChilds(index);
	}
	else
	{
		ClearChilds(index);
	}
}

bool Scene::IsAlive(EntityID entity) const noexcept
{
	if (entity.IsNull())
		return false;

	COMMON_ASSERT(entity.Index() < m_entityCount);

	const auto index = entity.Index();

	return m_entities[index].version == entity.Version();
}

void Scene::Clear() noexcept
{
	if (m_entityCount == 0)
		return;

	if (m_liveEntityCount == 0)
		return;

	auto index = m_entitySkips[0];
	while (index != m_entityCount)
	{
		auto& e = m_entities[index];

		e.version = m_entitySkips[index] ? e.version : ((e.version + 1) & 0xFFF);
		m_entitySkips[index] = 1;

		index = index + m_entitySkips[index + 1] + 1;
	}

	m_entitySkips[0] = m_entityCount;
	m_entitySkips[m_entityCount - 1] = m_entityCount;
	m_entities[0].freeListNext = -1;
	m_entities[0].freeListPrev = -1;
	m_freeListHead = 0;
	m_liveEntityCount = 0;
	m_deadEntityCount = m_entityCount;
	m_names.clear();
	m_uniqueIds.clear();
	m_dirtyTransforms.clear();
}

void Scene::SetName(EntityID entity, string name)
{
	COMMON_ASSERT(IsAlive(entity));
	COMMON_ASSERT(!m_names.contains(name));

	auto& e = m_entities[entity.Index()];

	if (!e.name.empty())
	{
		m_names.erase(e.name);
	}

	if (name.empty())
	{
		e.name = {};
	}
	else
	{
		const auto result = m_names.insert_or_assign(name, entity);
		e.name = result.first->first;
	}
}

string_view Scene::GetName(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return m_entities[entity.Index()].name;
}

Transform Scene::GetTransform(EntityID entity) noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return { this, entity };
}

CTransform Scene::GetTransform(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return { this, entity };
}

void Scene::SetParent(EntityID entity, EntityID parent)
{
	COMMON_ASSERT(IsAlive(entity));
	COMMON_ASSERT(entity != parent);
	COMMON_ASSERT(!parent || IsAlive(parent));
	COMMON_ASSERT(!parent || HasParent(parent, entity));

	auto& e = m_entities[entity.Index()];

	// No changes
	if (e.parent == parent)
		return;

	if (e.parent)
		RemoveChild(e.parent.Index(), entity);

	e.parent = parent;
	if (parent)
	{
		m_hierarchy[parent.Index()].emplace_back(entity);

		if (GetFromID(parent).hasDirtyTransform)
		{
			SetDirtyWithNewParent(entity);
			return;
		}
	}
	SetDirty(entity);
}

EntityID Scene::GetParent(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return EntityID{ m_entities[entity.Index()].parent };
}

array_view<EntityID> Scene::GetChildren(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	const auto iter = m_hierarchy.find(entity.Index());

	if (iter == m_hierarchy.end())
		return {};

	return iter->second;
}

void Scene::SetUniqueID(EntityID entity, const uuid& id)
{
	COMMON_ASSERT(IsAlive(entity));
	COMMON_ASSERT(!m_uniqueIds.contains(id));

	auto& e = m_entities[entity.Index()];

	if (e.uniqueId)
	{
		m_uniqueIds.erase(*e.uniqueId);
	}

	const auto result = m_uniqueIds.insert_or_assign(id, entity);
	e.uniqueId = &result.first->first;
}

const uuid* Scene::GetUniqueID(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return m_entities[entity.Index()].uniqueId;
}

uint32_t Scene::LiveEntities() const noexcept
{
	return m_liveEntityCount;
}

uint32_t Scene::DeadEntities() const noexcept
{
	return m_deadEntityCount;
}

void Scene::AddCreateCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onCreate)
{
	m_createHandlers.emplace_back(managerPtr, std::move(onCreate));
}

void Scene::AddDestroyCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onDestroy)
{
	m_destroyHandlers.emplace_back(managerPtr, std::move(onDestroy));
}

void Scene::AddTransformChangeCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity, const matrix4x3& matrix)> onTransformChange)
{
	m_transformChangeHandlers.emplace_back(managerPtr, std::move(onTransformChange));
}

void Scene::RemoveAllCallbacks(void* managerPtr) noexcept
{
	mst::iterate_remove(m_createHandlers, [managerPtr](auto& p)
	{
		return p.first == managerPtr;
	});

	mst::iterate_remove(m_destroyHandlers, [managerPtr](auto& p)
	{
		return p.first == managerPtr;
	});

	mst::iterate_remove(m_transformChangeHandlers, [managerPtr](auto& p)
	{
		return p.first == managerPtr;
	});
}

void Scene::BuildMatrices() noexcept
{
	for (auto it = m_dirtyTransforms.rbegin(); it != m_dirtyTransforms.rend(); ++it)
	{
		const auto entity = *it;

		auto& e = m_entities[entity.Index()];

		e.globalMatrix = std::visit([this, &e](const auto& o) { return GenerateLocalMatrixImpl(e.position, o); }, e.orientation);
		e.hasDirtyTransform = false;

		if (e.parent)
		{
			const auto& parentMatrix = GetGlobalMatrix(e.parent);

			e.globalMatrix = parentMatrix * e.globalMatrix;
		}
	}

	m_dirtyTransforms.clear();
}

const matrix4x3& Scene::GetGlobalMatrix(EntityID entity) const noexcept
{
	COMMON_ASSERT(IsAlive(entity));

	return m_entities[entity.Index()].globalMatrix;
}

void Scene::RemoveChild(int32_t parent, EntityID child) noexcept
{
	auto& childs = m_hierarchy.at(parent);

	for (auto it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == child)
		{
			childs.erase(it);
			return;
		}
	}
}

void Scene::ClearChilds(int32_t parent)
{
	const auto iter = m_hierarchy.find(parent);

	if (iter != m_hierarchy.end())
	{
		for (auto& child : iter->second)
		{
			auto& e = m_entities[child.Index()];
			e.parent = EntityID::Null;
			SetDirty(child);
		}

		m_hierarchy.erase(iter);
	}
}

void Scene::DestroyChilds(int32_t parent)
{
	const auto iter = m_hierarchy.find(parent);

	if (iter != m_hierarchy.end())
	{
		for (auto& child : iter->second)
		{
			const auto index = child.Index();

			DestroyAndAddToFreeList(index);

			DestroyChilds(index);
		}
		m_hierarchy.erase(iter);
	}
}

void Scene::DestroyAndAddToFreeList(int32_t entityIndex)
{
	auto& e = m_entities[entityIndex];

	e.version = (e.version + 1) & 0xFFF;
	if (!e.name.empty())
	{
		m_names.erase(e.name);
	}
	if (e.uniqueId)
	{
		m_uniqueIds.erase(*e.uniqueId);
	}

	AddToFreeList(entityIndex);
}

void Scene::SetDirty(EntityID entity)
{
	const auto entityIndex = entity.Index();

	auto& e = m_entities[entityIndex];

	// Already set to dirty, ignore
	if (e.hasDirtyTransform)
		return;

	e.hasDirtyTransform = true;
	const auto iter = m_hierarchy.find(entityIndex);

	if (iter != m_hierarchy.end())
	{
		for (auto child : iter->second)
		{
			SetDirty(child);
		}
	}

	m_dirtyTransforms.push_back(entity);
}

void Scene::SetDirtyWithNewParent(EntityID entity)
{
	SetDirty(entity);

	auto parent = GetFromID(entity).parent;

	while (parent && GetFromID(parent).hasDirtyTransform)
	{
		m_dirtyTransforms.push_back(parent);

		parent = GetFromID(parent).parent;
	}
}

Scene::EntityData& Scene::GetFromID(EntityID entity) noexcept
{
	return m_entities[entity.Index()];
}

const Scene::EntityData& Scene::GetFromID(EntityID entity) const noexcept
{
	return m_entities[entity.Index()];
}

int32_t Scene::RemoveFromFreeList() noexcept
{
	const auto entityIndex = m_freeListHead;

	if (m_freeListHead == -1) [[unlikely]]
		return m_freeListHead;

	++m_liveEntityCount;
	--m_deadEntityCount;

	const auto lastIndex = m_entityCount - 1;

	const bool containsRight = entityIndex != lastIndex && m_entitySkips[entityIndex + 1] != 0;

	if (containsRight)
	{
		m_entitySkips[entityIndex + m_entitySkips[entityIndex] - 1] =
			m_entitySkips[entityIndex + 1] = m_entitySkips[entityIndex] - 1;
		m_entities[entityIndex + 1].freeListPrev = -1;
		m_entities[entityIndex + 1].freeListNext = m_entities[entityIndex].freeListNext;
		m_freeListHead = entityIndex + 1;
	}
	else
	{
		m_freeListHead = m_entities[entityIndex].freeListNext;
		if (m_freeListHead != -1)
		{
			m_entities[m_freeListHead].freeListPrev = -1;
		}
	}

	m_entitySkips[entityIndex] = 0;
	return entityIndex;
}

void Scene::AddToFreeList(int32_t entityIndex) noexcept
{
	--m_liveEntityCount;
	++m_deadEntityCount;

	const auto lastIndex = m_entityCount - 1;

	const uint8_t containsLeft = entityIndex != 0 && m_entitySkips[entityIndex - 1] != 0;
	const uint8_t containsRight = entityIndex != lastIndex && m_entitySkips[entityIndex + 1] != 0;

	switch (containsLeft | (containsRight << 1))
	{
		// Contains none
	[[likely]] case 0:
	{
		m_entitySkips[entityIndex] = 1;

		// Set as start of the free list
		m_entities[entityIndex].freeListNext = m_freeListHead;
		if (m_freeListHead != -1)
		{
			m_entities[m_freeListHead].freeListPrev = entityIndex;
		}
		m_freeListHead = entityIndex;
		break;
	}
		// Contains left
	case 1:
	{
		const auto skipStart = entityIndex - m_entitySkips[entityIndex - 1];
		const auto skipCount = entityIndex - skipStart + 1;

		m_entitySkips[skipStart] = skipCount;
		m_entitySkips[entityIndex] = skipCount;
		break;
	}
		// Contains right
	case 2:
	{
		const auto skipEnd = entityIndex + m_entitySkips[entityIndex + 1];

		const auto skipCount = skipEnd - entityIndex + 1;

		m_entitySkips[entityIndex] = skipCount;
		m_entitySkips[skipEnd] = skipCount;

		// Replace previous start of skip range
		const auto followingNext = m_entities[entityIndex + 1].freeListNext;
		const auto followingPrev = m_entities[entityIndex + 1].freeListPrev;

		m_entities[entityIndex].freeListPrev = followingPrev;
		m_entities[entityIndex].freeListNext = followingNext;

		if (followingPrev != -1)
		{
			m_entities[followingPrev].freeListNext = entityIndex;
		}
		else
		{
			m_freeListHead = entityIndex;
		}
		if (followingNext != -1)
		{
			m_entities[followingNext].freeListPrev = entityIndex;
		}
		break;
	}
		// Contains both
	case 3:
	{
		const auto skipStart = entityIndex - m_entitySkips[entityIndex - 1];
		const auto skipEnd = entityIndex + m_entitySkips[entityIndex + 1];

		const auto skipCount = skipEnd - skipStart + 1;

		m_entitySkips[entityIndex] = 1;
		m_entitySkips[skipStart] = skipCount;
		m_entitySkips[skipEnd] = skipCount;

		// Remove right side start node from free list
		const auto followingNext = m_entities[entityIndex + 1].freeListNext;
		const auto followingPrev = m_entities[entityIndex + 1].freeListPrev;

		if (followingPrev != -1)
		{
			m_entities[followingPrev].freeListNext = followingNext;
		}
		else
		{
			m_freeListHead = followingNext;
		}

		if (followingNext != -1)
		{
			m_entities[followingNext].freeListPrev = followingPrev;
		}

		break;
	}
	}
}

void Scene::ResetParentForDestroy(EntityID entity) noexcept
{
	auto& e = m_entities[entity.Index()];

	if (e.parent)
		RemoveChild(e.parent.Index(), entity);
}

int32_t Scene::CreateNew()
{
	++m_liveEntityCount;

	if (m_entityCount == m_capacity) [[unlikely]]
	{
		if (m_capacity == 0) [[unlikely]]
		{
			m_capacity = 256;
			m_entities = reinterpret_cast<EntityData*>(mst::aligned_malloc(m_capacity * sizeof(EntityData), alignof(EntityData)));
			m_entitySkips = reinterpret_cast<int32_t*>(mst::aligned_realloc(m_entitySkips, m_capacity * sizeof(int32_t) + sizeof(int32_t), 64));
			m_entitySkips[0] = 0;
		}
		else
		{
			m_capacity <<= 1;

			auto newEntities = reinterpret_cast<EntityData*>(mst::aligned_malloc(m_capacity * sizeof(EntityData), alignof(EntityData)));

			for (int32_t i = 0; i < m_entityCount; ++i)
			{
				new (newEntities + i) EntityData(std::move(m_entities[i]));

				m_entities[i].~EntityData();
			}

			mst::aligned_free(m_entities);

			m_entities = newEntities;

			m_entitySkips = reinterpret_cast<int32_t*>(mst::aligned_realloc(m_entitySkips, m_capacity * sizeof(int32_t) + sizeof(int32_t), 64));
		}
	}

	m_entitySkips[m_entityCount + 1] = 0;
	new (m_entities + m_entityCount) EntityData();

	return m_entityCount++;
}

bool Scene::HasParent(EntityID entity, EntityID parent) const noexcept
{
	auto p = GetFromID(entity).parent;
	while (p)
	{
		if (parent == p) [[unlikely]]
			return true;

		p = GetFromID(p).parent;
	}

	return false;
}

matrix4x3 Scene::GenerateLocalMatrixImpl(const float3& position, const Scene::EulerAngles& angles) const noexcept
{
	return matrix4x3(position).rotated(angles.x, angles.y, angles.z, angles.order);
}

matrix4x3 Scene::GenerateLocalMatrixImpl(const float3& position, const quaternion& quat) const noexcept
{
	return { position, quat };
}