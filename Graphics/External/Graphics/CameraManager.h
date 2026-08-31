#pragma once

namespace CainEngine::Graphics {

class CameraManager final
{
	COMMON_DECLARE_NON_COPY(CameraManager);

	struct EntityData;
	struct PerspectiveData;
	struct OrthographicData;
	using Scene = EntitySystem::Scene;
	using EntityID = EntitySystem::EntityID;

public:
	// ctor & dtor
	CameraManager(Scene& scene);
	~CameraManager();

public:
	// Main functionality

	void addCamera(EntityID entity);

	void removeCamera(EntityID entity) noexcept;

	void moveCamera(EntityID entityFrom, EntityID entityTo);

	[[nodiscard]] bool hasCamera(EntityID entity) const noexcept;

public:
	// Component settings

	void setPerspectiveProjection(
		EntityID entity, degrees fov, float nearDepth, float farDepth) noexcept;
	void setOrthographicProjection(EntityID entity, const float2& min, const float2& max,
		float nearDepth, float farDepth) noexcept;

private:
	// Internal functionality

	[[nodiscard]] uint16_t addRenderData();
	void removeRenderData(uint16_t index) noexcept;

	matrix createProjectionMatrix(const EntityData& entityData, const float2& viewport) const;
	matrix createProjectionMatrixImpl(const PerspectiveData& data, const float2& viewport) const;
	matrix createProjectionMatrixImpl(const OrthographicData& data, const float2& viewport) const;

private:
	// Member variables

	struct PerspectiveData
	{
		radians fov;
		float nearDepth;
		float farDepth;
	};

	struct OrthographicData
	{
		float2 min, max;
		float nearDepth, farDepth;
	};

	struct EntityData
	{
		uint16_t renderDataIndex;
		std::variant<PerspectiveData, OrthographicData> projectionData;
	};

	struct RenderData
	{
		matrix4x4 projection, inverseProjection;
		matrix4x4 view, inverseView;
	};

	absl::flat_hash_map<int32_t, EntityData> m_entityDatas;
	std::vector<RenderData> m_renderDatas;
	std::vector<int32_t> m_renderDataToEntities;
	Scene& m_scene;
};

}; // namespace CainEngine::Graphics