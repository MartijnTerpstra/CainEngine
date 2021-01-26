#pragma once

namespace CainEngine::Graphics {

class ShaderManager final
{
	COMMON_DECLARE_NON_COPY(ShaderManager);
private:
	// ctor & dtor
	ShaderManager();
	~ShaderManager();

public:
	// Main functionality

	static void Init(Renderer* renderer, Common::Source& source);

	static void Exit();

	static API::VertexShader* GetVertexShader(const string& name);

	static API::PixelShader* GetPixelShader(const string& name);

private:
	// Member variables

	static vector<pair<stack_string<64>, API::VertexShader*>> m_vertexShaders;
	static vector<pair<stack_string<64>, API::PixelShader*>> m_pixelShaders;
	static vector<pair<stack_string<64>, API::ComputeShader*>> m_computeShaders;
	static vector<pair<stack_string<64>, API::GeometryShader*>> m_geometryShaders;
	static vector<pair<stack_string<64>, API::HullShader*>> m_hullShaders;
	static vector<pair<stack_string<64>, API::DomainShader*>> m_domainShaders;
};

}