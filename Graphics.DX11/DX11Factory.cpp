#include "Precomp.h"

#include "DX11Texture.h"
#include "DX11Factory.h"

#include "InputLayoutResolver.h"
#include "DX11Renderer.h"

#include "DX11Shader.h"
#include "VertexData.h"
#include "EnumConverter.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

CainEngine::Graphics::API::VertexData::~VertexData()
{
}

DX11Factory::DX11Factory()
{
}

DX11Factory::~DX11Factory()
{
}

API::VertexShader* DX11Factory::CreateVertexShader(API::IRenderer* renderer, API::CompiledShaderData&& shaderData)
{
	COMMON_CALLSTACK_CALL;

	const auto dx11Renderer = (DX11Renderer*)renderer;

	auto inputRegisterHash = mst::hash64(memory_view(shaderData.inputRegisters.data(), shaderData.inputRegisters.size()));

	dx11Renderer->AddVertexShaderInputRegisters(inputRegisterHash, shaderData.inputRegisters);

	const auto iter = m_vertexShaders.emplace(dx11Renderer->D3DDevice(), move(shaderData), move(shaderData.byteCode), inputRegisterHash);

	return iter.ptr();
}

API::PixelShader* DX11Factory::CreatePixelShader(API::IRenderer* renderer, API::CompiledShaderData&& shaderData)
{
	COMMON_CALLSTACK_CALL;

	const auto dx11Renderer = (DX11Renderer*)renderer;

	const auto iter = m_pixelShaders.emplace(dx11Renderer->D3DDevice(), move(shaderData), shaderData.byteCode);

	return iter.ptr();
}

API::VertexData* DX11Factory::CreateVertexData(API::IRenderer* renderer, const VertexDataCreationInfo& creationInfo)
{
	COMMON_CALLSTACK_CALL;

	const auto dx11Renderer = (DX11Renderer*)renderer;

	if (creationInfo.positions.empty())
	{
		Common::Error("Invalid call to CreateVertexData: positions is empty");
		return nullptr;
	}

	if (creationInfo.indices.empty())
	{
		Common::Error("Invalid call to CreateVertexData: indices is empty");
		return nullptr;
	}

	if (creationInfo.positions.size() != creationInfo.normals.size() ||
		creationInfo.positions.size() != creationInfo.uvs.size())
	{
		Common::Error("Invalid call to CreateVertexData: positions is empty");
		return nullptr;
	}

	const auto device = dx11Renderer->D3DDevice();

	const auto vertexData = m_vertexDatas.emplace();

	struct VertexItem
	{
		float3 position;
		float3 normal;
		float2 uv;
	};

	vector<VertexItem> vertices(creationInfo.positions.size());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = creationInfo.positions[i];
		vertices[i].normal = creationInfo.normals[i];
		vertices[i].uv = creationInfo.uvs[i];
	}
	vertexData->stridesAndOffsets.push_back(sizeof(VertexItem));
	vertexData->stridesAndOffsets.push_back(0);
	vertexData->topology = EnumConverter::Convert(creationInfo.topology);
	vertexData->vertexBuffers.emplace_back();

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(VertexItem));
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA data = {};

		data.pSysMem = vertices.data();

		CHECK_HRESULT(device->CreateBuffer(&desc, &data, mst::initialize(vertexData->vertexBuffers[0])));
	}


	vertexData->indexCount = (uint32_t)creationInfo.indices.size();
	vertexData->indexFormat = DXGI_FORMAT_R16_UINT;

	if (std::any_of(creationInfo.indices.begin(), creationInfo.indices.end(), [](uint32_t index) { return index >= USHRT_MAX; }))
	{
		vertexData->indexFormat = DXGI_FORMAT_R32_UINT;

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = static_cast<UINT>(creationInfo.indices.size() * sizeof(uint32_t));
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA data = {};

		data.pSysMem = creationInfo.indices.data();

		CHECK_HRESULT(device->CreateBuffer(&desc, &data, mst::initialize(vertexData->indexBuffer)));
	}
	else
	{
		vector<uint16_t> indices(creationInfo.indices.size());

		for (size_t i = 0; i < indices.size(); ++i)
		{
			indices[i] = static_cast<uint16_t>(creationInfo.indices[i]);
		}

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(uint16_t));
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA data = {};

		data.pSysMem = indices.data();

		CHECK_HRESULT(device->CreateBuffer(&desc, &data, mst::initialize(vertexData->indexBuffer)));
	}

	vertexData->vertexLayout.reserve(3);
	vertexData->vertexLayout.push_back(API::VertexBufferDesc(API::ShaderSemanticName::Position, 0, API::ShaderVariableType::Float, 3, 0));
	vertexData->vertexLayout.push_back(API::VertexBufferDesc(API::ShaderSemanticName::Normal, 0, API::ShaderVariableType::Float, 3, 12));
	vertexData->vertexLayout.push_back(API::VertexBufferDesc(API::ShaderSemanticName::Texcoord, 0, API::ShaderVariableType::Float, 2, 24));

	std::sort(vertexData->vertexLayout.begin(), vertexData->vertexLayout.end(), [](const API::VertexBufferDesc& l, const API::VertexBufferDesc& r)
	{
		if (l.semanticName != r.semanticName)
			return l.semanticName < r.semanticName;

		return l.semanticIndex < r.semanticIndex;
	});

	vertexData->vertexLayoutHash = mst::hash64(memory_view(vertexData->vertexLayout.begin(), vertexData->vertexLayout.size()));

	return vertexData.ptr();
}

shared_ptr<API::IBuffer> DX11Factory::CreateBuffer(API::IRenderer* renderer, ResourceType type, size_t dataSize, memory_view memory, uint structSize, API::Usage usage)
{
	return nullptr;
}

pair<int32_t, API::ITexture*> DX11Factory::CreateTexture(API::IRenderer* renderer, ResourceType type, const uint3& size, PixelFormat format, flag<API::BindFlags> bindFlags, API::Usage usage, uint arraySize, uint mipLevels)
{
	return { -1, nullptr };
}

pair<int32_t, API::ITexture*> DX11Factory::CreateTexture(API::IRenderer* renderer, ResourceType type, const uint3& size, array_view<API::PixelData> initialData, PixelFormat format, flag<API::BindFlags> bindFlags, API::Usage usage, uint arraySize, uint mipLevels)
{
	COMMON_CALLSTACK_CALL;

	const auto dx11Renderer = (DX11Renderer*)renderer;

	auto device = dx11Renderer->D3DDevice();

	inlined_vector<D3D11_SUBRESOURCE_DATA, 16> subResourceData(initialData.size());

	for (size_t i = 0; i < subResourceData.size(); ++i)
	{
		subResourceData[i].pSysMem = initialData[i].pixels;
		subResourceData[i].SysMemPitch = initialData[i].pitch;
		subResourceData[i].SysMemSlicePitch = initialData[i].pitch * size.y;
	}

	const D3D11_SUBRESOURCE_DATA* dataPtr = nullptr;
	if (!initialData.empty())
	{
		dataPtr = subResourceData.data();
	}

	com_ptr<ID3D11Resource> resource;
	switch (type)
	{
	case CainEngine::Graphics::ResourceType::VertexBuffer:
	case CainEngine::Graphics::ResourceType::IndexBuffer:
	case CainEngine::Graphics::ResourceType::ConstantBuffer:
	case CainEngine::Graphics::ResourceType::ByteAddressBuffer:
	case CainEngine::Graphics::ResourceType::StructuredBuffer:
		Common::FatalError("Invalid argument: type");
	case CainEngine::Graphics::ResourceType::Tex1D:
	{
		D3D11_TEXTURE1D_DESC desc = {};
		desc.ArraySize = arraySize;
		desc.BindFlags = EnumConverter::Convert(bindFlags);
		desc.Format = EnumConverter::Convert(format);
		desc.MipLevels = mipLevels;
		desc.Width = size.x;

		auto [u, c] = EnumConverter::Convert(usage);
		desc.Usage = u;
		desc.CPUAccessFlags = c;

		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(device->CreateTexture1D(&desc, dataPtr, mst::initialize(tex)));

		resource = tex;
	}
	case CainEngine::Graphics::ResourceType::Tex2D:
		break;
	case CainEngine::Graphics::ResourceType::TexCube:
		break;
	case CainEngine::Graphics::ResourceType::Tex3D:
		break;
	default:
		break;
	}

	const auto iter = m_textures.emplace(std::move(resource));

	return { iter.idx(), iter.ptr() };
}

void DX11Factory::Clear() noexcept
{
	m_vertexDatas.clear();
	m_vertexShaders.clear();
	m_pixelShaders.clear();
	m_textures.clear();
}