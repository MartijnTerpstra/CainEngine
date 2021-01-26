#pragma once

#include <Common.h>
#include <Graphics.h>

namespace CainEngine::Editor::ShaderCompiler {

struct ShaderDefine
{
	string key;
	string value;
};

struct ShaderCompilation
{
	string source;
	string name;
	Graphics::ShaderType shaderType;
	optional<string> entryPoint;
	vector<string> renderers;
	vector<ShaderDefine> defines;
};

bool CompileShaders(const vector<ShaderCompilation>& shaders, const char* sourceDirectory, const char* targetDirectory, bool optization);

}