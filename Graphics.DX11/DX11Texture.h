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

	ResourceType Type() const override;
	API::Usage Usage() const override;
	uint3 Size() const override;
	PixelFormat Format() const override;
	uint ArraySize() const override;
	flag<API::BindFlags> BindFlags() const override;

private:
	// Member variables

	com_ptr<ID3D11Resource> m_texture;
};

}