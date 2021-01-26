#include "Precomp.h"

using namespace ::CainEngine::Graphics;

vector<pair<stack_string<64>, API::VertexShader*>> ShaderManager::m_vertexShaders;
vector<pair<stack_string<64>, API::PixelShader*>> ShaderManager::m_pixelShaders;
vector<pair<stack_string<64>, API::ComputeShader*>> ShaderManager::m_computeShaders;
vector<pair<stack_string<64>, API::GeometryShader*>> ShaderManager::m_geometryShaders;
vector<pair<stack_string<64>, API::HullShader*>> ShaderManager::m_hullShaders;
vector<pair<stack_string<64>, API::DomainShader*>> ShaderManager::m_domainShaders;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Init(Renderer* renderer, Common::Source& source)
{
	const auto impl = renderer->Implementation();
	const auto factory = impl->GetFactory();

	size_t index = 0;

	uint32_t magicNumber = source.ReadUint();
	if (magicNumber != API::SHADER_MAGIC_NUMBER)
	{
		Common::Error("Unable to parse shader file: magic number missmatch");
		return;
	}

	uint32_t rendererID = source.ReadUint();
	if (rendererID != renderer->ID())
	{
		Common::Error("Unable to parse shader file: unknown renderer ID");
		return;
	}

	auto metadatas = source.ReadVector<API::CompiledShaderMetaData>();

	vector<API::CompiledShaderData> shaders(metadatas.size());

	for (size_t i = 0; i < metadatas.size(); ++i)
	{
		auto& metadata = metadatas[i];

		API::CompiledShaderData data;

		data.byteCode = source.ReadVector<byte>();
		data.buffers = source.ReadVector<ShaderBufferInfo>();
		data.textures = source.ReadVector<ShaderTextureInfo>();
		data.inputRegisters = source.ReadVector<API::ShaderRegisterInfo>();
		data.outputRegisters = source.ReadVector<API::ShaderRegisterInfo>();

		switch (metadata.shaderType)
		{
		case ShaderType::Vertex:
			m_vertexShaders.push_back({ metadata.shaderName, factory->CreateVertexShader(impl, move(data)) });
			break;
		case ShaderType::Pixel:
			m_pixelShaders.push_back({ metadata.shaderName, factory->CreatePixelShader(impl, move(data)) });
			break;
		default:
			Common::FatalError("metadata.shaderType corrupted/unimplemented");
		}
	}
}

void ShaderManager::Exit()
{
	m_vertexShaders.clear();
	m_pixelShaders.clear();
	m_computeShaders.clear();
	m_geometryShaders.clear();
	m_hullShaders.clear();
	m_domainShaders.clear();
}

API::VertexShader* ShaderManager::GetVertexShader(const string& name)
{
	COMMON_CALLSTACK_CALL;

	for (auto& shader : m_vertexShaders)
	{
		if (shader.first == name)
		{
			return shader.second;
		}
	}

	Common::Warning("VertexShader not found with name: '%s'", name);
	return nullptr;
}

API::PixelShader* ShaderManager::GetPixelShader(const string& name)
{
	COMMON_CALLSTACK_CALL;

	for (auto& shader : m_pixelShaders)
	{
		if (shader.first == name)
		{
			return shader.second;
		}
	}

	Common::Warning("PixelShader not found with name: '%s'", name);
	return nullptr;
}