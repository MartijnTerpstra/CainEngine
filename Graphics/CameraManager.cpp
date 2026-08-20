#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

CameraManager::CameraManager(Scene& scene)
	: m_scene(scene)
{
	m_scene.AddDestroyCallback(this, [this](Scene&, EntityID entity) {
		if(HasCamera(entity))
		{
			RemoveCamera(entity);
		}
	});
	m_scene.AddTransformChangeCallback(
		this, [this](Scene&, EntityID entity, const matrix3x4& matrix) {
			auto dataIt = m_entityDatas.find(entity.Index());
			if(dataIt == m_entityDatas.end())
			{
				return;
			}
			auto& renderData = m_renderDatas[dataIt->second.renderDataIndex];
			renderData.inverseView = matrix4x4(matrix, float4(0, 0, 0, 1));
			renderData.view = renderData.inverseView.get_inverse();
		});
}

CameraManager::~CameraManager()
{
	m_scene.RemoveAllCallbacks(this);
}

void CameraManager::AddCamera(EntityID entity)
{
	COMMON_ASSERT(!HasCamera(entity));

	const auto index = entity.Index();

	auto& data = m_entityDatas[index];

	data.renderDataIndex = AddRenderData();
	data.projectionData = PerspectiveData{ degrees(45), 0.1f, 10000.f };
}

void CameraManager::RemoveCamera(EntityID entity) noexcept
{
	COMMON_ASSERT(HasCamera(entity));

	const auto index = entity.Index();

	const auto renderDataIndex = m_entityDatas.extract(index).mapped().renderDataIndex;

	m_entityDatas.erase(index);

	RemoveRenderData(renderDataIndex);
}

void CameraManager::MoveCamera(EntityID entityFrom, EntityID entityTo)
{
	COMMON_ASSERT(HasCamera(entityTo));
	COMMON_ASSERT(HasCamera(entityFrom));

	const auto indexFrom = entityFrom.Index();
	const auto indexTo = entityTo.Index();

	m_entityDatas.emplace(indexTo, std::move(m_entityDatas.at(indexFrom)));
	m_entityDatas.erase(indexFrom);
}

bool CameraManager::HasCamera(EntityID entity) const noexcept
{
	COMMON_ASSERT(entity);

	return m_entityDatas.contains(entity.Index());
}

void CameraManager::SetPerspectiveProjection(
	EntityID entity, degrees fov, float nearDepth, float farDepth) noexcept
{
	COMMON_ASSERT(HasCamera(entity));

	const auto index = entity.Index();

	auto& data = m_entityDatas.at(index);

	data.projectionData = PerspectiveData{ fov, nearDepth, farDepth };
}

void CameraManager::SetOrthographicProjection(
	EntityID entity, const float2& min, const float2& max, float nearDepth, float farDepth) noexcept
{
	COMMON_ASSERT(HasCamera(entity));

	const auto index = entity.Index();

	auto& data = m_entityDatas.at(index);

	data.projectionData = OrthographicData{ min, max, nearDepth, farDepth };
}

uint16_t CameraManager::AddRenderData()
{
	COMMON_ASSERT(m_renderDatas.size() <= USHRT_MAX);

	const auto index = static_cast<uint16_t>(m_renderDatas.size());
	m_renderDatas.emplace_back();
	return index;
}

void CameraManager::RemoveRenderData(uint16_t index) noexcept
{
	COMMON_ASSERT(index < m_renderDatas.size());

	const auto lastIndex = m_renderDatas.size() - 1;

	if(index != lastIndex)
	{
		auto& entityData = m_entityDatas[m_renderDataToEntities.back()];
		entityData.renderDataIndex = index;
	}

	m_renderDatas.pop_back();
	m_renderDataToEntities.pop_back();
}

matrix CameraManager::CreateProjectionMatrix(
	const EntityData& entityData, const float2& viewport) const
{
	return std::visit([&](const auto& data) { return CreateProjectionMatrixImpl(data, viewport); },
		entityData.projectionData);
}

matrix CameraManager::CreateProjectionMatrixImpl(
	const PerspectiveData& data, const float2& viewport) const
{
	return matrix::create_perspective(
		data.fov, viewport.x, viewport.y, data.nearDepth, data.farDepth);
}

matrix CameraManager::CreateProjectionMatrixImpl(
	const OrthographicData& data, const float2& viewport) const
{
	return matrix::create_orthographic(
		aabb(float3(data.min, data.nearDepth), float3(data.max, data.farDepth)));
}