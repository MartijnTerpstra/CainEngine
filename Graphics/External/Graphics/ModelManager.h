#pragma once

namespace CainEngine::Graphics {

class ModelManager final
{
	COMMON_DECLARE_NON_COPY(ModelManager);

	using Scene = EntitySystem::Scene;
	using EntityID = EntitySystem::EntityID;
	struct ModelData;
public:
	// ctor & dtor
	ModelManager();
	~ModelManager();

public:
	// Main functionality

	[[nodiscard]] pair<int32_t, Material*> CreateMaterial(Renderer& renderer, API::VertexShader* vs, API::PixelShader* ps);

	[[nodiscard]] Material* GetMaterial(int32_t materialID) noexcept;

	void RemoveMaterial(int32_t materialID) noexcept;

	[[nodiscard]] pair<int32_t, Model*> CreateModel(Renderer& renderer, const VertexDataCreationInfo& vertexData);

	[[nodiscard]] Model* GetModel(int32_t modelID) noexcept;

	void RemoveModel(int32_t modelID) noexcept;

	void AddEntity(int32_t modelID, Scene& scene, EntityID entity);

	void RemoveEntity(EntityID entity) noexcept;

	[[nodiscard]] Model* GetModel(EntityID entity) noexcept;

	[[nodiscard]] bool HasModel(EntityID entity) const noexcept;

	[[nodiscard]] bool HasModel(int32_t modelID, EntityID entity) const noexcept;

	void AttachCallbacks(Scene& scene);

	[[nodiscard]] const colony<ModelData>& GetModels() const noexcept;

private:

	struct ModelData
	{
		Model model;
		inlined_vector<matrix4x4, 1> matrices;
		vector<EntityID> entities;
	};

	struct EntityData
	{
		int32_t modelID;
		float scale = 1;
		matrix4x3 matrix;
	};

	colony<ModelData> m_models;
	colony<Material> m_materials;
	hash_map<int32_t, EntityData> m_entityMapping;
};

}