#pragma once

namespace CainEngine::Editor {

class ShaderSourceManager
{
public:
	// ctor & dtor
	ShaderSourceManager();
	~ShaderSourceManager();

public:
	// Main functionality

	bool compileShaders();

private:
	// Member variables

	Common::JsonDeclaration<ShaderCompiler::ShaderCompilation> m_declaration;

};

}