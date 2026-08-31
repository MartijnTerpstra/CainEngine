#pragma once

namespace CainEngine::Graphics::DX11 {

class DX11Factory final : public API::IFactory
{
public:
	// ctor & dtor

	DX11Factory();
	~DX11Factory();

public:
	// IResourceFactory overrides

	API::VertexShader* createVertexShader(
		API::IRenderer* renderer, API::CompiledShaderData&& shaderData) override;

	API::PixelShader* createPixelShader(
		API::IRenderer* renderer, API::CompiledShaderData&& shaderData) override;

	API::VertexData* createVertexData(
		API::IRenderer* renderer, const VertexDataCreationInfo& creationInfo) override;

	std::shared_ptr<API::IBuffer> createBuffer(API::IRenderer* renderer, ResourceType type,
		size_t dataSize, memory_view memory = memory_view(nullptr, 0), uint32_t structSize = 0,
		API::Usage usage = API::Usage::Default) override;
	std::pair<int32_t, API::ITexture*> createTexture(API::IRenderer* renderer, ResourceType type,
		const uint3& size, PixelFormat format = PixelFormat::Default,
		flag<API::BindFlags> bindFlags = API::BindFlags::ShaderResource,
		API::Usage usage = API::Usage::Default, uint32_t arraySize = 1,
		uint32_t mipLevels = 1) override;
	std::pair<int32_t, API::ITexture*> createTexture(API::IRenderer* renderer, ResourceType type,
		const uint3& size, array_view<API::PixelData> initialData,
		PixelFormat format = PixelFormat::Default,
		flag<API::BindFlags> bindFlags = API::BindFlags::ShaderResource,
		API::Usage usage = API::Usage::Default, uint32_t arraySize = 1,
		uint32_t mipLevels = 1) override;

	void clear() noexcept;

private:
	// Member variables
	colony<API::VertexData> m_vertexDatas;
	colony<API::VertexShader> m_vertexShaders;
	colony<API::PixelShader> m_pixelShaders;
	colony<DX11Texture> m_textures;
};

} // namespace CainEngine::Graphics::DX11