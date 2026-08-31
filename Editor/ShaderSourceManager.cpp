#include "Precomp.h"

#include "ShaderSourceManager.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Editor;
using namespace ::CainEngine::Editor::ShaderCompiler;

ShaderSourceManager::ShaderSourceManager()
{
	std::map<std::string, Graphics::ShaderType> shaderTypeMap;
	shaderTypeMap["Vertex"] = Graphics::ShaderType::Vertex;
	shaderTypeMap["Pixel"] = Graphics::ShaderType::Pixel;
	shaderTypeMap["Compute"] = Graphics::ShaderType::Compute;
	shaderTypeMap["Geometry"] = Graphics::ShaderType::Geometry;
	shaderTypeMap["Hull"] = Graphics::ShaderType::Hull;
	shaderTypeMap["Domain"] = Graphics::ShaderType::Domain;

	m_declaration.addMember("source", &ShaderCompilation::source);
	m_declaration.addMember("name", &ShaderCompilation::name);
	m_declaration.addMember("shaderType", &ShaderCompilation::shaderType, std::move(shaderTypeMap));
	m_declaration.addMember("entryPoint", &ShaderCompilation::entryPoint);
	m_declaration.addMember("renderers", &ShaderCompilation::renderers);

	Common::JsonDeclaration<ShaderDefine> defineDecl;
	defineDecl.addMember("key", &ShaderDefine::key);
	defineDecl.addMember("value", &ShaderDefine::value);
	m_declaration.addMember("defines", &ShaderCompilation::defines, defineDecl, true);
}

ShaderSourceManager::~ShaderSourceManager()
{ }

bool ShaderSourceManager::compileShaders()
{
	Common::JsonParser parser;

	auto config = parser.parseArray(CONTENT_DIRECTORY "/Config/Shaders.json", m_declaration);

	if(config)
	{
		return ShaderCompiler::compileShaders(
			*config, CONTENT_DIRECTORY "/Shaders source", CONTENT_DIRECTORY "/Shaders", false);
	}
	return false;
}