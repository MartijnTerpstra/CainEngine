#pragma once

namespace CainEngine::Graphics::DX11 {

class DX11RenderContext final : public API::IRenderContext
{
public:
	// ctor & dtor

	DX11RenderContext(DX11Renderer* renderer, ID3D11DeviceContext* context);
	~DX11RenderContext();

public:
	// IRenderContext overrides

	void setViewport(const Viewport& vp, float depthMin, float depthMax) override;
	void setVertexData(const API::VertexData& vertexData) override;
	void setVertexShader(API::VertexShader* shader) override;
	void setPixelShader(API::PixelShader* shader) override;
	void draw(uint32_t vertexCount, uint32_t startOffset) override;
	void drawIndexed(uint32_t indexCount, uint32_t startOffset) override;

private:
	// Member variables

	ID3D11DeviceContext* const m_context;
	API::VertexShader* m_vertexShader = nullptr;
	const API::VertexData* m_vertexData = nullptr;
	DX11Renderer* const m_renderer;
};

}