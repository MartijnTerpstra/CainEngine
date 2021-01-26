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
	CameraManager();
	~CameraManager();

public:
	// Main functionality

	void AddCamera(EntityID entity);

	void RemoveCamera(EntityID entity) noexcept;

	void MoveCamera(EntityID entityFrom, EntityID entityTo);

	[[nodiscard]] bool HasCamera(EntityID entity) const noexcept;

	void AttachCallbacks(Scene& scene);

public:
	// Component settings

	void SetPerspectiveProjection(EntityID entity, degrees fov, float nearDepth, float farDepth) noexcept;
	void SetOrthographicProjection(EntityID entity, const float2& min, const float2& max, float nearDepth, float farDepth) noexcept;

private:
	// Internal functionality

	[[nodiscard]] uint16_t AddRenderData();
	void RemoveRenderData(uint16_t index) noexcept;

	matrix CreateProjectionMatrix(const EntityData& entityData, const float2& viewport) const;
	matrix CreateProjectionMatrixImpl(const PerspectiveData& data, const float2& viewport) const;
	matrix CreateProjectionMatrixImpl(const OrthographicData& data, const float2& viewport) const;

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
		variant<PerspectiveData, OrthographicData> projectionData;
	};

	struct RenderData
	{
		matrix4x4 projection, inverseProjection;
		matrix4x4 view, inverseView;
	};

	sparse_set<EntityData, int32_t> m_entityDatas;
	vector<RenderData> m_renderDatas;
};

};