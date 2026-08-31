#pragma once

namespace CainEngine::Graphics::DX11 {

class DX11Texture : public API::ITexture
{
public:
	// ctor & dtor

	DX11Texture(com_ptr<ID3D11Resource>&& texture);
	~DX11Texture();

public:
	// ITexture overrides

	ResourceType type() const override;
	API::Usage usage() const override;
	uint3 size() const override;
	PixelFormat format() const override;
	uint32_t arraySize() const override;
	flag<API::BindFlags> bindFlags() const override;

private:
	// Member variables

	com_ptr<ID3D11Resource> m_texture;
};

} // namespace CainEngine::Graphics::DX11