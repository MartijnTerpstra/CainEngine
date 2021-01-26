#include "Precomp.h"

#include "ICompiler.h"
#include "DXCompiler.h"

using namespace CainEngine::Graphics;
using namespace CainEngine::Editor::ShaderCompiler;

template<typename T>
static const T* ExtractFromMapping(const std::map<string, T>& mapping, const string& key, const char* variableName);

bool CainEngine::Editor::ShaderCompiler::CompileShaders(const vector<ShaderCompilation>& shaders, const char* sourceDirectory, const char* targetDirectory, bool optization)
{
	std::map<string, unique_ptr<ICompiler>> rendererTypeMap;
	rendererTypeMap["DX11"] = CreateDX11Compiler();
	rendererTypeMap["DX12"] = CreateDX12Compiler();

	std::map<ICompiler*, vector<tuple<string, ShaderType, API::CompiledShaderData>>> compiledShaders;

	bool failure = false;

	for (auto& shader : shaders)
	{
		if (shader.name.empty())
		{
			Common::Error("name: required");
			failure = true;
			continue;
		}

		if (shader.name.length() > 63)
		{
			Common::Error("name: must be max. 63 characters long, name will be truncated");
			failure = true;
		}

		if (shader.shaderType == (ShaderType)0)
		{
			failure = true;
			continue;
		}

		auto entryPoint = shader.entryPoint.value_or("main");

		if (shader.renderers.empty())
		{
			Common::Error("renderers: required");
			failure = true;
			continue;
		}

		for (auto& renderer : shader.renderers)
		{
			auto compiler = ExtractFromMapping(rendererTypeMap, renderer, "Renderer");
			if (!compiler)
				continue;

			compiledShaders[compiler->get()].emplace_back(shader.name.substr(0, 63), shader.shaderType, (*compiler)->Compile(sourceDirectory, shader.source.c_str(), shader.shaderType, entryPoint.c_str(), shader.defines, optization));
		}
	}

	for (auto& byRenderer : compiledShaders)
	{
		vector<API::CompiledShaderMetaData> metadata(byRenderer.second.size());

		std::ofstream outfile(string(targetDirectory) + "/" + byRenderer.first->RendererType() + ".shaders", std::ios::binary);

		auto m = API::SHADER_MAGIC_NUMBER;
		auto id = byRenderer.first->RendererID(); 

		outfile.write(reinterpret_cast<const char*>(&m), sizeof(m));
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(id));

		uint32_t shaderCount = (uint32_t)metadata.size();

		outfile.write(reinterpret_cast<const char*>(&shaderCount), sizeof(shaderCount));

		int64_t metaStart = outfile.tellp();

		outfile.write(reinterpret_cast<const char*>(metadata.data()), sizeof(API::CompiledShaderMetaData) * metadata.size());

		for (size_t i = 0; i < metadata.size(); ++i)
		{
			metadata[i].shaderName = std::get<0>(byRenderer.second[i]);
			metadata[i].shaderType = std::get<1>(byRenderer.second[i]);
			metadata[i].memoryOffset = outfile.tellp();

			auto& data = std::get<2>(byRenderer.second[i]);

			uint32_t size = (uint32_t)data.byteCode.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outfile.write(reinterpret_cast<const char*>(data.byteCode.data()), data.byteCode.size());

			size = (uint32_t)data.buffers.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outfile.write(reinterpret_cast<const char*>(data.buffers.data()), data.buffers.size() * sizeof(ShaderBufferInfo));

			size = (uint32_t)data.textures.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outfile.write(reinterpret_cast<const char*>(data.textures.data()), data.textures.size() * sizeof(ShaderTextureInfo));

			size = (uint32_t)data.inputRegisters.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outfile.write(reinterpret_cast<const char*>(data.inputRegisters.data()), data.inputRegisters.size() * sizeof(API::ShaderRegisterInfo));

			size = (uint32_t)data.outputRegisters.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outfile.write(reinterpret_cast<const char*>(data.outputRegisters.data()), data.outputRegisters.size() * sizeof(API::ShaderRegisterInfo));
		}

		outfile.seekp(metaStart);

		outfile.write(reinterpret_cast<const char*>(metadata.data()), sizeof(API::CompiledShaderMetaData)* metadata.size());

	}

	return !failure;
}

template<typename T>
const T* ExtractFromMapping(const std::map<string, T>& mapping, const string& key, const char* variableName)
{
	auto foundIt = mapping.find(key);

	if (foundIt == mapping.end())
	{
		string supportedValues;

		Common::Error("%s: '%s' in not a valid value, supported values: ", variableName, key, supportedValues);
		return nullptr;
	}

	return &foundIt->second;
}