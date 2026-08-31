#pragma once

namespace CainEngine::Editor::ShaderCompiler {

class ICompiler
{
public:
	// ctor & dtor

	inline ICompiler() = default;
	inline virtual ~ICompiler();

public:
	// Main functionality

	virtual Graphics::API::CompiledShaderData compile(const char* sourceDirectory,
		const char* filePath,
		Graphics::ShaderType shaderType,
		const char* entryPoint,
		const std::vector<ShaderDefine>& defines,
		bool optimize) const = 0;

	virtual std::string rendererType() const = 0;

	virtual uint32_t rendererId() const = 0;

};

inline ICompiler::~ICompiler() = default;

}