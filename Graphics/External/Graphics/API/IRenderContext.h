#pragma once

namespace CainEngine::Graphics::API {

class IRenderContext
{
public:
	inline IRenderContext() = default;
	inline virtual ~IRenderContext();

public:
	// Main functionality
	
	virtual void setViewport(const Viewport& vp, float depthMin = 0, float depthMax = 1) = 0;

	virtual void setVertexData(const VertexData& vertexData) = 0;

	virtual void setVertexShader(VertexShader* shader) = 0;

	virtual void setPixelShader(PixelShader* shader) = 0;

	virtual void draw(uint32_t vertexCount, uint32_t startOffset = 0) = 0;

	virtual void drawIndexed(uint32_t indexCount, uint32_t startOffset = 0) = 0;

}; // class IRenderContext

inline IRenderContext::~IRenderContext() = default;

};