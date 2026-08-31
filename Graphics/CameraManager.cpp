#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

CameraManager::CameraManager(Scene& scene)
	: m_scene(scene)
{
	m_scene.addDestroyCallback(this, [this](Scene&, EntityID entity) {
		if(hasCamera(entity))
		{
			removeCamera(entity);
		}
	});
	m_scene.addTransformChangeCallback(
		this, [this](Scene&, EntityID entity, const matrix3x4& matrix) {
			auto dataIt = m_entityDatas.find(entity.index());
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
	m_scene.removeAllCallbacks(this);
}

void CameraManager::addCamera(EntityID entity)
{
	COMMON_ASSERT(!hasCamera(entity));

	const auto index = entity.index();

	auto& data = m_entityDatas[index];

	data.renderDataIndex = addRenderData();
	data.projectionData = PerspectiveData{ degrees(45), 0.1f, 10000.f };
}

void CameraManager::removeCamera(EntityID entity) noexcept
{
	COMMON_ASSERT(hasCamera(entity));

	const auto index = entity.index();

	const auto renderDataIndex = m_entityDatas.extract(index).mapped().renderDataIndex;

	m_entityDatas.erase(index);

	removeRenderData(renderDataIndex);
}

void CameraManager::moveCamera(EntityID entityFrom, EntityID entityTo)
{
	COMMON_ASSERT(hasCamera(entityTo));
	COMMON_ASSERT(hasCamera(entityFrom));

	const auto indexFrom = entityFrom.index();
	const auto indexTo = entityTo.index();

	m_entityDatas.emplace(indexTo, std::move(m_entityDatas.at(indexFrom)));
	m_entityDatas.erase(indexFrom);
}

bool CameraManager::hasCamera(EntityID entity) const noexcept
{
	COMMON_ASSERT(entity);

	return m_entityDatas.contains(entity.index());
}

void CameraManager::setPerspectiveProjection(
	EntityID entity, degrees fov, float nearDepth, float farDepth) noexcept
{
	COMMON_ASSERT(hasCamera(entity));

	const auto index = entity.index();

	auto& data = m_entityDatas.at(index);

	data.projectionData = PerspectiveData{ fov, nearDepth, farDepth };
}

void CameraManager::setOrthographicProjection(
	EntityID entity, const float2& min, const float2& max, float nearDepth, float farDepth) noexcept
{
	COMMON_ASSERT(hasCamera(entity));

	const auto index = entity.index();

	auto& data = m_entityDatas.at(index);

	data.projectionData = OrthographicData{ min, max, nearDepth, farDepth };
}

uint16_t CameraManager::addRenderData()
{
	COMMON_ASSERT(m_renderDatas.size() <= USHRT_MAX);

	const auto index = static_cast<uint16_t>(m_renderDatas.size());
	m_renderDatas.emplace_back();
	return index;
}

void CameraManager::removeRenderData(uint16_t index) noexcept
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

matrix CameraManager::createProjectionMatrix(
	const EntityData& entityData, const float2& viewport) const
{
	return std::visit([&](const auto& data) { return createProjectionMatrixImpl(data, viewport); },
		entityData.projectionData);
}

matrix CameraManager::createProjectionMatrixImpl(
	const PerspectiveData& data, const float2& viewport) const
{
	return matrix::create_perspective(
		data.fov, viewport.x, viewport.y, data.nearDepth, data.farDepth);
}

matrix CameraManager::createProjectionMatrixImpl(
	const OrthographicData& data, const float2& viewport) const
{
	return matrix::create_orthographic(
		aabb(float3(data.min, data.nearDepth), float3(data.max, data.farDepth)));
}