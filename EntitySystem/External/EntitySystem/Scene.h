#pragma once

namespace CainEngine::EntitySystem {

class Scene
{
	COMMON_DECLARE_NON_COPY(Scene);

	struct EntityData;
	template<bool>
	friend class TransformImpl;

public:
	// ctor & dtor

	Scene();
	~Scene();

public:
	// Main functionality

	EntityID create();

	EntityID create(std::string name);

	EntityID find(std::string_view name) const noexcept;

	EntityID find(const uuid& id) const noexcept;

	void destroy(EntityID entity, bool recursive = false);

	bool isAlive(EntityID entity) const noexcept;

	void clear() noexcept;

	template<typename Func>
	void forEach(Func func) noexcept;

	template<typename Func, typename Arg>
	void forEach(Func func, Arg&& arg) noexcept;

	template<typename Func>
	void forEach(Func func) const noexcept;

	template<typename Func, typename Arg>
	void forEach(Func func, Arg&& arg) const noexcept;

public:
	// Statistics

	uint32_t liveEntities() const noexcept;
	uint32_t capacity() const noexcept;
	uint32_t deadEntities() const noexcept;

public:
	// Built-in components

	void setName(EntityID entity, std::string name);

	std::string_view getName(EntityID entity) const noexcept;

	Transform getTransform(EntityID entity) noexcept;

	CTransform getTransform(EntityID entity) const noexcept;

	void setParent(EntityID entity, EntityID parent);

	EntityID getParent(EntityID entity) const noexcept;

	array_view<EntityID> getChildren(EntityID entity) const noexcept;

	void setUniqueId(EntityID entity, const uuid& id);

	const uuid* getUniqueId(EntityID entity) const noexcept;

public:
	// Event management

	void addCreateCallback(
		void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onCreate);
	void addDestroyCallback(
		void* managerPtr, std::function<void(Scene& scene, EntityID entity)> onDestroy);
	void addTransformChangeCallback(void* managerPtr,
		std::function<void(Scene& scene, EntityID entity, const matrix3x4& matrix)>
			onTransformChange);
	void removeAllCallbacks(void* managerPtr) noexcept;

public:
	// Rendering functionality

	void buildMatrices() noexcept;
	const matrix3x4& getGlobalMatrix(EntityID entity) const noexcept;

private:
	// Internal functionality

	void removeChild(int32_t parentIndex, EntityID child) noexcept;
	void clearChilds(int32_t parentIndex);
	void destroyChilds(int32_t parentIndex);
	void destroyAndAddToFreeList(int32_t entityIndex);
	void setDirty(EntityID entity);
	void setDirtyWithNewParent(EntityID entity);
	EntityData& getFromId(EntityID entity) noexcept;
	const EntityData& getFromId(EntityID entity) const noexcept;
	int32_t removeFromFreeList() noexcept;
	void addToFreeList(int32_t entityIndex) noexcept;
	void resetParentForDestroy(EntityID entityIndex) noexcept;
	int32_t createNew();
	bool hasParent(EntityID entity, EntityID parent) const noexcept;

	struct EulerAngles
	{
		radians x, y, z;
		euler_rotation_order order;
	};

	matrix3x4 generateLocalMatrixImpl(
		const float3& position, const EulerAngles& angles) const noexcept;
	matrix3x4 generateLocalMatrixImpl(
		const float3& position, const quaternion& quat) const noexcept;

private:
	// Member variables

	struct alignas(64) EntityData
	{
		// uint32_t unused
		EntityID parent;		 // 4 bytes - offset 4
		flag<EntityFlags> flags; //	4 bytes - offset 8
		uint16_t version = 0;	 // 2 bytes - offset 12
		bool hasDirtyTransform;	 // 1 bytes - offset 14
		// uint8_t unused
		matrix3x4 globalMatrix; // 48 bytes - offset 16
		// --- cache line split
		float3 position;								   // 12 bytes - offset 64
		std::variant<quaternion, EulerAngles> orientation; // 20 bytes - offset 76
		std::string_view name;							   // 8 bytes - offset 96
		const uuid* uniqueId;							   // 8 bytes - offset 104
		int32_t freeListPrev;							   // 4 bytes - offset 112
		int32_t freeListNext;
	};

	using ChildContainer = inlined_vector<EntityID, 4>;

	static_assert(sizeof(EntityData) <= 128, "EntityData should be 128 bytes in size");
	static_assert(sizeof(ChildContainer) <= 24, "ChildContainer should be max 16 bytes");

	EntityData* m_entities = nullptr;
	int32_t m_freeListHead = -1;
	flat_hash_map<int32_t, inlined_vector<EntityID, 4>> m_hierarchy;
	node_hash_map<std::string, EntityID> m_names;
	node_hash_map<uuid, EntityID> m_uniqueIds;
	std::vector<EntityID> m_dirtyTransforms;
	int32_t* m_entitySkips = nullptr;
	std::vector<std::pair<void*, std::function<void(Scene& scene, EntityID entity)>>>
		m_destroyHandlers;
	std::vector<std::pair<void*, std::function<void(Scene& scene, EntityID entity)>>>
		m_createHandlers;
	std::vector<std::pair<void*,
		std::function<void(Scene& scene, EntityID entity, const matrix3x4& matrix)>>>
		m_transformChangeHandlers;
	int32_t m_entityCount = 0, m_capacity = 0;
	uint32_t m_liveEntityCount = 0, m_deadEntityCount = 0;
};

}; // namespace CainEngine::EntitySystem