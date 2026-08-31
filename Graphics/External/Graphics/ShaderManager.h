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

	static void init(Renderer* renderer, Common::Source& source);

	static void exit();

	static API::VertexShader* getVertexShader(const std::string& name);

	static API::PixelShader* getPixelShader(const std::string& name);

private:
	// Member variables

	static std::vector<std::pair<static_string<64>, API::VertexShader*>> m_vertexShaders;
	static std::vector<std::pair<static_string<64>, API::PixelShader*>> m_pixelShaders;
	static std::vector<std::pair<static_string<64>, API::ComputeShader*>> m_computeShaders;
	static std::vector<std::pair<static_string<64>, API::GeometryShader*>> m_geometryShaders;
	static std::vector<std::pair<static_string<64>, API::HullShader*>> m_hullShaders;
	static std::vector<std::pair<static_string<64>, API::DomainShader*>> m_domainShaders;
};

} // namespace CainEngine::Graphics