#pragma once

namespace CainEngine::Graphics {

class Model final
{
	COMMON_DECLARE_NON_COPY(Model);

	struct SubMesh;

public:
	// ctor & dtor

	Model();
	~Model();

public:
	// Main functionality

	void setVertexData(API::VertexData* vertexData) noexcept;

	void addSubMesh(uint32_t indexOffset, uint32_t indexCount, int32_t materialIndex);

	[[nodiscard]] array_view<SubMesh> subMeshes() const noexcept;

	[[nodiscard]] const API::VertexData& vertexData() const noexcept;

private:
	// Member variables

	struct SubMesh
	{
		uint32_t indexOffset, indexCount;
		int32_t materialIndex;
	};

	API::VertexData* m_vertexData;
	inlined_vector<SubMesh, 4> m_subMeshes;
};

} // namespace CainEngine::Graphics