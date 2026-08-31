#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

ModelManager::ModelManager()
{ }

ModelManager::~ModelManager()
{ }

std::pair<int32_t, Material*> ModelManager::createMaterial(
	Renderer& renderer, API::VertexShader* vs, API::PixelShader* ps)
{
	const auto iter = m_materials.emplace(vs, ps);

	return { iter.idx(), iter.ptr() };
}

Material* ModelManager::getMaterial(int32_t materialID) noexcept
{
	return &m_materials[materialID];
}

void ModelManager::removeMaterial(int32_t materialID) noexcept
{
	m_materials.erase(materialID);
}

std::pair<int32_t, Model*> ModelManager::createModel(
	Renderer& renderer, const VertexDataCreationInfo& vertexData)
{
	const auto impl = renderer.implementation();
	const auto factory = impl->getFactory();

	const auto iter = m_models.emplace();

	iter->model.setVertexData(factory->createVertexData(impl, vertexData));

	return { iter.idx(), &iter->model };
}

[[nodiscard]] Model* ModelManager::getModel(int32_t modelID) noexcept
{
	return &m_models[modelID].model;
}

void ModelManager::removeModel(int32_t modelID) noexcept
{
	m_models.erase(modelID);
}

void ModelManager::addEntity(int32_t modelID, Scene& scene, EntityID entity)
{
	COMMON_ASSERT(m_entityMapping.find(entity.index()) == m_entityMapping.end());

	const auto& iter = m_entityMapping.emplace(
		std::piecewise_construct, std::make_tuple(entity.index()), std::make_tuple());

	auto& data = iter.first->second;

	data.modelID = modelID;
	data.matrix = scene.getGlobalMatrix(entity);

	auto& model = m_models[modelID];

	model.matrices.emplace_back(data.matrix, float4(0, 0, 0, 1));
}

void ModelManager::removeEntity(EntityID entity) noexcept
{
	COMMON_ASSERT(m_entityMapping.find(entity.index()) != m_entityMapping.end());

	const auto& iter = m_entityMapping.find(entity.index());

	auto& model = m_models[iter->second.modelID];

	m_entityMapping.erase(iter);

	for(size_t i = 0; i < model.entities.size(); ++i)
	{
		if(model.entities[i] == entity)
		{
			model.entities.erase(model.entities.begin() + i);
			model.matrices.erase(model.matrices.begin() + i);
			return;
		}
	}
}

Model* ModelManager::getModel(EntityID entity) noexcept
{
	const auto& iter = m_entityMapping.find(entity.index());

	if(iter == m_entityMapping.end())
		return nullptr;

	return getModel(iter->second.modelID);
}

bool ModelManager::hasModel(EntityID entity) const noexcept
{
	return m_entityMapping.find(entity.index()) != m_entityMapping.end();
}

bool ModelManager::hasModel(int32_t modelID, EntityID entity) const noexcept
{
	const auto& iter = m_entityMapping.find(entity.index());

	if(iter == m_entityMapping.end())
		return false;

	return modelID == iter->second.modelID;
}

void ModelManager::attachCallbacks(Scene& scene)
{
	scene.addDestroyCallback(this, [this](Scene& scene, EntityID entity) {
		if(hasModel(entity))
			removeEntity(entity);
	});

	scene.addTransformChangeCallback(
		this, [this](Scene& scene, EntityID entity, const matrix3x4& matrix) {
			const auto& iter = m_entityMapping.find(entity.index());

			if(iter == m_entityMapping.end())
				return;

			auto& data = iter->second;

			data.matrix = matrix;

			auto& model = m_models[iter->second.modelID];

			for(size_t i = 0; i < model.entities.size(); ++i)
			{
				if(model.entities[i] == entity)
				{
					model.matrices[i] = matrix4x4(matrix, float4(0, 0, 0, 1));

					if(data.scale != 1)
					{
						model.matrices[i].scale(data.scale);
					}
					return;
				}
			}
		});
}

const colony<ModelManager::ModelData>& ModelManager::getModels() const noexcept
{
	return m_models;
}