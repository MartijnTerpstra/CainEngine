#pragma once

namespace CainEngine::Graphics::API {

class IRenderContext
{
public:
	inline IRenderContext() = default;
	inline virtual ~IRenderContext();

public:
	// Main functionality
	
	virtual void SetViewport(const Viewport& vp, float depthMin = 0, float depthMax = 1) = 0;

	virtual void SetVertexData(const VertexData& vertexData) = 0;

	virtual void SetVertexShader(VertexShader* shader) = 0;

	virtual void SetPixelShader(PixelShader* shader) = 0;

	virtual void Draw(uint32_t vertexCount, uint32_t startOffset = 0) = 0;

	virtual void DrawIndexed(uint32_t indexCount, uint32_t startOffset = 0) = 0;

}; // class IRenderContext

inline IRenderContext::~IRenderContext() = default;

};