#pragma once

#include <Common.h>
#include <Graphics.h>

namespace CainEngine::Editor::ShaderCompiler {

struct ShaderDefine
{
	std::string key;
	std::string value;
};

struct ShaderCompilation
{
	std::string source;
	std::string name;
	Graphics::ShaderType shaderType;
	std::optional<std::string> entryPoint;
	std::vector<std::string> renderers;
	std::vector<ShaderDefine> defines;
};

bool compileShaders(const std::vector<ShaderCompilation>& shaders, const char* sourceDirectory,
	const char* targetDirectory, bool optization);

} // namespace CainEngine::Editor::ShaderCompiler