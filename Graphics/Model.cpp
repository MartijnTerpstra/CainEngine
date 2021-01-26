#include "Precomp.h"

using namespace ::CainEngine::Graphics;

Model::Model()
{
}

Model::~Model()
{
}

void Model::SetVertexData(API::VertexData* vertexData) noexcept
{
	m_vertexData = vertexData;
}

void Model::AddSubMesh(uint32_t indexOffset, uint32_t indexCount, int32_t materialIndex)
{
	auto& subMesh = m_subMeshes.emplace_back();

	subMesh.indexOffset = indexOffset;
	subMesh.indexCount = indexCount;
	subMesh.materialIndex = materialIndex;
}

array_view<Model::SubMesh> Model::SubMeshes() const noexcept
{
	return m_subMeshes;
}

const API::VertexData& Model::VertexData() const noexcept
{
	return *m_vertexData;
}