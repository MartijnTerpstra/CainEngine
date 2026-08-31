#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

std::vector<std::pair<static_string<64>, API::VertexShader*>> ShaderManager::m_vertexShaders;
std::vector<std::pair<static_string<64>, API::PixelShader*>> ShaderManager::m_pixelShaders;
std::vector<std::pair<static_string<64>, API::ComputeShader*>> ShaderManager::m_computeShaders;
std::vector<std::pair<static_string<64>, API::GeometryShader*>> ShaderManager::m_geometryShaders;
std::vector<std::pair<static_string<64>, API::HullShader*>> ShaderManager::m_hullShaders;
std::vector<std::pair<static_string<64>, API::DomainShader*>> ShaderManager::m_domainShaders;

ShaderManager::ShaderManager()
{ }

ShaderManager::~ShaderManager()
{ }

void ShaderManager::init(Renderer* renderer, Common::Source& source)
{
	const auto impl = renderer->implementation();
	const auto factory = impl->getFactory();

	size_t index = 0;

	uint32_t magicNumber = source.readUint();
	if(magicNumber != API::SHADER_MAGIC_NUMBER)
	{
		Common::error("Unable to parse shader file: magic number missmatch");
		return;
	}

	uint32_t rendererID = source.readUint();
	if(rendererID != renderer->id())
	{
		Common::error("Unable to parse shader file: unknown renderer ID");
		return;
	}

	auto metadatas = source.readVector<API::CompiledShaderMetaData>();

	std::vector<API::CompiledShaderData> shaders(metadatas.size());

	for(size_t i = 0; i < metadatas.size(); ++i)
	{
		auto& metadata = metadatas[i];

		API::CompiledShaderData data;

		data.byteCode = source.readVector<uint8_t>();
		data.buffers = source.readVector<ShaderBufferInfo>();
		data.textures = source.readVector<ShaderTextureInfo>();
		data.inputRegisters = source.readVector<API::ShaderRegisterInfo>();
		data.outputRegisters = source.readVector<API::ShaderRegisterInfo>();

		switch(metadata.shaderType)
		{
		case ShaderType::Vertex:
			m_vertexShaders.push_back(
				{ metadata.shaderName, factory->createVertexShader(impl, std::move(data)) });
			break;
		case ShaderType::Pixel:
			m_pixelShaders.push_back(
				{ metadata.shaderName, factory->createPixelShader(impl, std::move(data)) });
			break;
		default:
			Common::fatalError("metadata.shaderType corrupted/unimplemented");
		}
	}
}

void ShaderManager::exit()
{
	m_vertexShaders.clear();
	m_pixelShaders.clear();
	m_computeShaders.clear();
	m_geometryShaders.clear();
	m_hullShaders.clear();
	m_domainShaders.clear();
}

API::VertexShader* ShaderManager::getVertexShader(const std::string& name)
{
	COMMON_CALLSTACK_CALL;

	for(auto& shader : m_vertexShaders)
	{
		if(shader.first == name)
		{
			return shader.second;
		}
	}

	Common::warning("VertexShader not found with name: '%s'", name);
	return nullptr;
}

API::PixelShader* ShaderManager::getPixelShader(const std::string& name)
{
	COMMON_CALLSTACK_CALL;

	for(auto& shader : m_pixelShaders)
	{
		if(shader.first == name)
		{
			return shader.second;
		}
	}

	Common::warning("PixelShader not found with name: '%s'", name);
	return nullptr;
}