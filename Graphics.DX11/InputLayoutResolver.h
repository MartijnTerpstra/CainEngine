#pragma once

namespace CainEngine::Graphics::DX11 {

class InputLayoutResolver
{
public:
	// ctor & dtor
	InputLayoutResolver() = default;
	~InputLayoutResolver() = default;

public:
	// Main functionality

	ID3D11InputLayout* Resolve(ID3D11Device* device, API::VertexShader* vs, uint64_t vertexLayoutHash, array_view<API::VertexBufferDesc> vertexLayout);

	void AddVertexShaderInputRegisters(uint64_t inputRegisterHash, array_view<API::ShaderRegisterInfo> inputRegisters);

	void Clear();

private:
	// Member variables

	struct LayoutInfo
	{
		inlined_vector<API::ShaderRegisterInfo, 16> inputRegisters;
		vector<pair<inlined_vector<API::VertexBufferDesc, 16>, com_ptr<ID3D11InputLayout>>> resolvedLayouts;
	};

	mst::arraymap<pair<uint64_t, uint64_t>, ID3D11InputLayout*> m_directResolves;
	mst::arraymap<uint64_t, LayoutInfo> m_shaders;
};

}