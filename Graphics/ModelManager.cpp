#include "Precomp.h"

using namespace ::CainEngine::Graphics;

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

pair<int32_t, Material*> ModelManager::CreateMaterial(Renderer& renderer, API::VertexShader* vs, API::PixelShader* ps)
{
	const auto iter = m_materials.emplace(vs, ps);

	return { iter.idx(), iter.ptr() };
}

Material* ModelManager::GetMaterial(int32_t materialID) noexcept
{
	return &m_materials[materialID];
}

void ModelManager::RemoveMaterial(int32_t materialID) noexcept
{
	m_materials.erase(materialID);
}

pair<int32_t, Model*> ModelManager::CreateModel(Renderer& renderer, const VertexDataCreationInfo& vertexData)
{
	const auto impl = renderer.Implementation();
	const auto factory = impl->GetFactory();

	const auto iter = m_models.emplace();

	iter->model.SetVertexData(factory->CreateVertexData(impl, vertexData));

	return { iter.idx(), &iter->model };
}

[[nodiscard]] Model* ModelManager::GetModel(int32_t modelID) noexcept
{
	return &m_models[modelID].model;
}

void ModelManager::RemoveModel(int32_t modelID) noexcept
{
	m_models.erase(modelID);
}

void ModelManager::AddEntity(int32_t modelID, Scene& scene, EntityID entity)
{
	COMMON_ASSERT(m_entityMapping.find(entity.Index()) == m_entityMapping.end());

	const auto& iter = m_entityMapping.emplace(std::piecewise_construct, std::make_tuple(entity.Index()), std::make_tuple());

	auto& data = iter.first->second;

	data.modelID = modelID;
	data.matrix = scene.GetGlobalMatrix(entity);

	auto& model = m_models[modelID];

	model.matrices.emplace_back(data.matrix, float4(0, 0, 0, 1));
}

void ModelManager::RemoveEntity(EntityID entity) noexcept
{
	COMMON_ASSERT(m_entityMapping.find(entity.Index()) != m_entityMapping.end());

	const auto& iter = m_entityMapping.find(entity.Index());

	auto& model = m_models[iter->second.modelID];

	m_entityMapping.erase(iter);

	for (size_t i = 0; i < model.entities.size(); ++i)
	{
		if (model.entities[i] == entity)
		{
			model.entities.erase(model.entities.begin() + i);
			model.matrices.erase(model.matrices.begin() + i);
			return;
		}
	}
}

Model* ModelManager::GetModel(EntityID entity) noexcept
{
	const auto& iter = m_entityMapping.find(entity.Index());

	if (iter == m_entityMapping.end())
		return nullptr;

	return GetModel(iter->second.modelID);
}

bool ModelManager::HasModel(EntityID entity) const noexcept
{
	return m_entityMapping.find(entity.Index()) != m_entityMapping.end();
}

bool ModelManager::HasModel(int32_t modelID, EntityID entity) const noexcept
{
	const auto& iter = m_entityMapping.find(entity.Index());

	if (iter == m_entityMapping.end())
		return false;

	return modelID == iter->second.modelID;
}

void ModelManager::AttachCallbacks(Scene& scene)
{
	scene.AddDestroyCallback(this, [this](Scene& scene, EntityID entity)
	{
		if (HasModel(entity))
			RemoveEntity(entity);
	});

	scene.AddTransformChangeCallback(this, [this](Scene& scene, EntityID entity, const matrix4x3& matrix)
	{
		const auto& iter = m_entityMapping.find(entity.Index());

		if (iter == m_entityMapping.end())
			return;

		auto& data = iter->second;

		data.matrix = matrix;

		auto& model = m_models[iter->second.modelID];

		for (size_t i = 0; i < model.entities.size(); ++i)
		{
			if (model.entities[i] == entity)
			{
				model.matrices[i] = matrix4x4(matrix, float4(0, 0, 0, 1));
				
				if (data.scale != 1)
				{
					model.matrices[i].scale(data.scale);
				}
				return;
			}
		}
	});
}

const colony<ModelManager::ModelData>& ModelManager::GetModels() const noexcept
{
	return m_models;
}