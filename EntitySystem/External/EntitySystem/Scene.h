#pragma once

namespace CainEngine::EntitySystem {

class Transform;
class CTransform;

class Scene
{
	COMMON_DECLARE_NON_COPY(Scene);

	struct EntityData;
	friend class Transform;
	friend class CTransform;
public:
	// ctor & dtor

	Scene();
	~Scene();

public:
	// Main functionality

	EntityID Create();

	EntityID Create(string name);

	EntityID Find(string_view name) const noexcept;

	EntityID Find(const uuid& id) const noexcept;

	void Destroy(EntityID entity, bool recursive = false);

	bool IsAlive(EntityID entity) const noexcept;

	void Clear() noexcept;

	template<typename Func>
	void ForEach(Func func) noexcept;

	template<typename Func, typename Arg>
	void ForEach(Func func, Arg&& arg) noexcept;

	template<typename Func>
	void ForEach(Func func) const noexcept;

	template<typename Func, typename Arg>
	void ForEach(Func func, Arg&& arg) const noexcept;

public:
	// Statistics

	uint32_t LiveEntities() const noexcept;
	uint32_t Capacity() const noexcept;
	uint32_t DeadEntities() const noexcept;

public:
	// Built-in components

	void SetName(EntityID entity, string name);

	string_view GetName(EntityID entity) const noexcept;

	Transform GetTransform(EntityID entity) noexcept;

	CTransform GetTransform(EntityID entity) const noexcept;

	void SetParent(EntityID entity, EntityID parent);

	EntityID GetParent(EntityID entity) const noexcept;

	array_view<EntityID> GetChildren(EntityID entity) const noexcept;

	void SetUniqueID(EntityID entity, const uuid& id);

	const uuid* GetUniqueID(EntityID entity) const noexcept;

public:
	// Event management

	void AddCreateCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onCreate);
	void AddDestroyCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onDestroy);
	void AddTransformChangeCallback(void* managerPtr, std::function<void(Scene& scene, EntityID entity, const matrix4x3& matrix)> onTransformChange);
	void RemoveAllCallbacks(void* managerPtr) noexcept;

public:
	// Rendering functionality

	void BuildMatrices() noexcept;
	const matrix4x3& GetGlobalMatrix(EntityID entity) const noexcept;

private:
	// Internal functionality

	void RemoveChild(int32_t parentIndex, EntityID child) noexcept;
	void ClearChilds(int32_t parentIndex);
	void DestroyChilds(int32_t parentIndex);
	void DestroyAndAddToFreeList(int32_t entityIndex);
	void SetDirty(EntityID entity);
	void SetDirtyWithNewParent(EntityID entity);
	EntityData& GetFromID(EntityID entity) noexcept;
	const EntityData& GetFromID(EntityID entity) const noexcept;
	int32_t RemoveFromFreeList() noexcept;
	void AddToFreeList(int32_t entityIndex) noexcept;
	void ResetParentForDestroy(EntityID entityIndex) noexcept;
	int32_t CreateNew();
	bool HasParent(EntityID entity, EntityID parent) const noexcept;

	struct EulerAngles
	{
		radians x, y, z;
		euler_rotation_order order;
	};

	matrix4x3 GenerateLocalMatrixImpl(const float3& position, const EulerAngles& angles) const noexcept;
	matrix4x3 GenerateLocalMatrixImpl(const float3& position, const quaternion& quat) const noexcept;

private:
	// Member variables

	struct alignas(64) EntityData
	{
		//uint32_t unused
		EntityID parent;
		flag<EntityFlags> flags;
		uint16_t version = 0;
		bool hasDirtyTransform;
		// uint8_t unused
		matrix4x3 globalMatrix;
		float3 position;
		std::variant<quaternion, EulerAngles> orientation;
		string_view name;
		const uuid* uniqueId;
		int32_t freeListPrev;
		int32_t freeListNext;
	};

	static_assert(128 == sizeof(EntityData), "EntityData should be 128 bytes in size");

	EntityData* m_entities = nullptr;
	int32_t m_entityCount = 0, m_capacity = 0;
	int32_t m_freeListHead = -1;
	flat_hash_map<int32_t, inlined_vector<EntityID, 16>> m_hierarchy;
	node_hash_map<string, EntityID> m_names;
	node_hash_map<uuid, EntityID> m_uniqueIds;
	vector<EntityID> m_dirtyTransforms;
	int32_t* m_entitySkips = nullptr;
	vector<pair<void*, std::function<void(Scene& scene, EntityID entity)>>> m_destroyHandlers;
	vector<pair<void*, std::function<void(Scene& scene, EntityID entity)>>> m_createHandlers;
	vector<pair<void*, std::function<void(Scene& scene, EntityID entity, const matrix4x3& matrix)>>> m_transformChangeHandlers;
	uint32_t m_liveEntityCount = 0, m_deadEntityCount = 0;
};

};