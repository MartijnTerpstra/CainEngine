#include "Precomp.h"

#include "InputLayoutResolver.h"

#include "DX11Shader.h"
#include "EnumConverter.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

static bool Matches(array_view<API::VertexBufferDesc> resolved, array_view<API::VertexBufferDesc> unresolved)
{
	for (auto& r : resolved)
	{
		bool found = false;
		for (auto& u : unresolved)
		{
			// Binary matchable
			if (memcmp(&r, &u, sizeof(r)) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}

	return true;
}

ID3D11InputLayout* InputLayoutResolver::Resolve(ID3D11Device* device, API::VertexShader* vs, uint64_t vertexLayoutHash, array_view<API::VertexBufferDesc> vertexLayout)
{
	const auto shaderHash = vs->InputRegisterHash();

	auto directResolveIt = m_directResolves.find({ shaderHash,vertexLayoutHash });

	if (directResolveIt != m_directResolves.end())
		return directResolveIt->second;

	auto& shader = m_shaders.at(shaderHash);

	for (auto& layout : shader.resolvedLayouts)
	{ // Try match to existing slot

		if (Matches(layout.first, vertexLayout))
		{
			m_directResolves.insert({ {shaderHash, vertexLayoutHash}, layout.second.get() });
			return layout.second.get();
		}
	}

	const auto& inputRegisters = shader.inputRegisters;

	inlined_vector<API::VertexBufferDesc, 16> resolvedLayout;
	resolvedLayout.reserve(inputRegisters.size());

	inlined_vector<D3D11_INPUT_ELEMENT_DESC, 16> descs;
	descs.reserve(inputRegisters.size());
	for (auto& r : inputRegisters)
	{
		D3D11_INPUT_ELEMENT_DESC desc = {};

		const API::VertexBufferDesc* v = nullptr;
		for (auto& vl : vertexLayout)
		{
			if (vl.semanticName == r.semanticName &&
				vl.semanticIndex == r.semanticIndex)
			{
				v = &vl;
				break;
			}
		}

		if (!v)
		{
			Common::Error("Unable to generate vertex layout, semantics do not match with the shader");
			return nullptr;
		}

		if (v->type != r.variableType || v->elementCount != r.variableElementCount)
		{
			Common::Error("Unable to generate vertex layout, semantics variable type missmatch with the shader");
			return nullptr;
		}

		desc.SemanticName = EnumConverter::Convert(r.semanticName);
		desc.SemanticIndex = r.semanticIndex;
		desc.Format = EnumConverter::Convert(r.variableType, r.variableElementCount);
		desc.InputSlot = v->vertexBufferIndex;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		desc.AlignedByteOffset = v->byteOffset;

		descs.push_back(desc);
		resolvedLayout.push_back(*v);
	}

	com_ptr<ID3D11InputLayout> newLayout;
	CHECK_HRESULT(device->CreateInputLayout(descs.data(), (UINT)descs.size(), vs->ByteCode().data(), vs->ByteCode().size(), mst::initialize(newLayout)));

	shader.resolvedLayouts.emplace_back(std::move(resolvedLayout), move(newLayout));

	m_directResolves.insert({ {shaderHash, vertexLayoutHash}, shader.resolvedLayouts.back().second.get() });
	return shader.resolvedLayouts.back().second.get();
}

void InputLayoutResolver::AddVertexShaderInputRegisters(uint64_t inputRegisterHash, array_view<API::ShaderRegisterInfo> inputRegisters)
{
	LayoutInfo info;

	info.inputRegisters.reserve(inputRegisters.size());

	for (auto& r : inputRegisters)
	{
		info.inputRegisters.push_back(r);
	}

	std::sort(info.inputRegisters.begin(), info.inputRegisters.end(), [](const API::ShaderRegisterInfo& l, const API::ShaderRegisterInfo& r)
	{
		if (l.semanticName != r.semanticName)
			return l.semanticName < r.semanticName;

		return l.semanticIndex < r.semanticIndex;
	});

	if (!m_shaders.contains(inputRegisterHash))
	{
		m_shaders.insert({ inputRegisterHash, move(info) });
	}
}

void InputLayoutResolver::Clear()
{
	m_directResolves.clear();
	m_shaders.clear();
}