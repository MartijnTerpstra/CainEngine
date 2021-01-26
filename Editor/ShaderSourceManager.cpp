#include "Precomp.h"

#include "ShaderSourceManager.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Editor;
using namespace ::CainEngine::Editor::ShaderCompiler;

ShaderSourceManager::ShaderSourceManager()
{
	std::map<string, Graphics::ShaderType> shaderTypeMap;
	shaderTypeMap["Vertex"] = Graphics::ShaderType::Vertex;
	shaderTypeMap["Pixel"] = Graphics::ShaderType::Pixel;
	shaderTypeMap["Compute"] = Graphics::ShaderType::Compute;
	shaderTypeMap["Geometry"] = Graphics::ShaderType::Geometry;
	shaderTypeMap["Hull"] = Graphics::ShaderType::Hull;
	shaderTypeMap["Domain"] = Graphics::ShaderType::Domain;

	m_declaration.AddMember("source", &ShaderCompilation::source);
	m_declaration.AddMember("name", &ShaderCompilation::name);
	m_declaration.AddMember("shaderType", &ShaderCompilation::shaderType, move(shaderTypeMap));
	m_declaration.AddMember("entryPoint", &ShaderCompilation::entryPoint);
	m_declaration.AddMember("renderers", &ShaderCompilation::renderers);

	Common::JsonDeclaration<ShaderDefine> defineDecl;
	defineDecl.AddMember("key", &ShaderDefine::key);
	defineDecl.AddMember("value", &ShaderDefine::value);
	m_declaration.AddMember("defines", &ShaderCompilation::defines, defineDecl, true);
}

ShaderSourceManager::~ShaderSourceManager()
{
}

bool ShaderSourceManager::CompileShaders()
{
	Common::JsonParser parser;

	auto config = parser.ParseArray(CONTENT_DIRECTORY "/Config/Shaders.json", m_declaration);

	if (config)
	{
		return ShaderCompiler::CompileShaders(*config, CONTENT_DIRECTORY "/Shaders source", CONTENT_DIRECTORY "/Shaders", false);
	}
	return false;
}