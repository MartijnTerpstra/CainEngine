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

	void SetVertexData(API::VertexData* vertexData) noexcept;

	void AddSubMesh(uint32_t indexOffset, uint32_t indexCount, int32_t materialIndex);

	[[nodiscard]] array_view<SubMesh> SubMeshes() const noexcept;

	[[nodiscard]] const API::VertexData& VertexData() const noexcept;

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

}