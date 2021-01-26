#pragma once

namespace CainEngine::Graphics::API {

class VertexData
{
public:

	~VertexData();

	inlined_vector<com_ptr<ID3D11Buffer>, 2> vertexBuffers;
	inlined_vector<UINT, 4> stridesAndOffsets;
	com_ptr<ID3D11Buffer> indexBuffer;
	DXGI_FORMAT indexFormat;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	uint32_t indexCount;
	uint64_t vertexLayoutHash;
	inlined_vector<VertexBufferDesc, 16> vertexLayout;
};

}