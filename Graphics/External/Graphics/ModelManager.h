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

	[[nodiscard]] std::pair<int32_t, Material*> createMaterial(
		Renderer& renderer, API::VertexShader* vs, API::PixelShader* ps);

	[[nodiscard]] Material* getMaterial(int32_t materialID) noexcept;

	void removeMaterial(int32_t materialID) noexcept;

	[[nodiscard]] std::pair<int32_t, Model*> createModel(
		Renderer& renderer, const VertexDataCreationInfo& vertexData);

	[[nodiscard]] Model* getModel(int32_t modelID) noexcept;

	void removeModel(int32_t modelID) noexcept;

	void addEntity(int32_t modelID, Scene& scene, EntityID entity);

	void removeEntity(EntityID entity) noexcept;

	[[nodiscard]] Model* getModel(EntityID entity) noexcept;

	[[nodiscard]] bool hasModel(EntityID entity) const noexcept;

	[[nodiscard]] bool hasModel(int32_t modelID, EntityID entity) const noexcept;

	void attachCallbacks(Scene& scene);

	[[nodiscard]] const colony<ModelData>& getModels() const noexcept;

private:
	struct ModelData
	{
		Model model;
		inlined_vector<matrix4x4, 1> matrices;
		std::vector<EntityID> entities;
	};

	struct EntityData
	{
		int32_t modelID;
		float scale = 1;
		matrix3x4 matrix;
	};

	colony<ModelData> m_models;
	colony<Material> m_materials;
	flat_hash_map<int32_t, EntityData> m_entityMapping;
};

} // namespace CainEngine::Graphics