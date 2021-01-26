#pragma once

namespace CainEngine::Graphics {

class Material final
{
public:
	// ctor & dtor
	
	Material(API::VertexShader* vs, API::PixelShader* ps);
	~Material();

public:
	// Main functionality

	void SetVariable(uint32_t index, const float4& value);
	void SetTexture(uint32_t index, const float4& value);

	API::VertexShader* VertexShader() const;
	API::PixelShader* PixelShader() const;

private:
	// Member variables

	API::VertexShader* m_vertexShader;
	API::PixelShader* m_pixelShader;
	inlined_vector<int32_t, 8> m_textures;
	shared_ptr<API::IBuffer> m_buffer;
	vector<int32_t> m_variableOffsets;
};

}