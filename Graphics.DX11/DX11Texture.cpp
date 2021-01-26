#include "Precomp.h"

#include "DX11Texture.h"

#include "EnumConverter.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

DX11Texture::DX11Texture(com_ptr<ID3D11Resource>&& texture)
	: m_texture(move(texture))
{
}

DX11Texture::~DX11Texture()
{
}


ResourceType DX11Texture::Type() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);
	
	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		return ResourceType::Tex1D;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		auto type = ResourceType::Tex2D;
		if ((desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) != 0)
		{
			type = ResourceType::TexCube;
		}
		return type;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		return ResourceType::Tex3D;
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}

API::Usage DX11Texture::Usage() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);

	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture1D>(mst::initialize(tex)));
		D3D11_TEXTURE1D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Usage);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Usage);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		com_ptr<ID3D11Texture3D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture3D>(mst::initialize(tex)));
		D3D11_TEXTURE3D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Usage);
	}
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}

uint3 DX11Texture::Size() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);

	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture1D>(mst::initialize(tex)));
		D3D11_TEXTURE1D_DESC desc;
		tex->GetDesc(&desc);

		return { desc.Width, 1, 1 };
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		return { desc.Width, desc.Height, 1 };
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		com_ptr<ID3D11Texture3D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture3D>(mst::initialize(tex)));
		D3D11_TEXTURE3D_DESC desc;
		tex->GetDesc(&desc);

		return { desc.Width, desc.Height, desc.Depth };
	}
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}

PixelFormat DX11Texture::Format() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);

	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture1D>(mst::initialize(tex)));
		D3D11_TEXTURE1D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Format);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Format);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		com_ptr<ID3D11Texture3D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture3D>(mst::initialize(tex)));
		D3D11_TEXTURE3D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::Convert(desc.Format);
	}
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}

uint DX11Texture::ArraySize() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);

	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture1D>(mst::initialize(tex)));
		D3D11_TEXTURE1D_DESC desc;
		tex->GetDesc(&desc);

		return desc.ArraySize;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		return desc.ArraySize;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		return 1;
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}

flag<API::BindFlags> DX11Texture::BindFlags() const
{
	COMMON_CALLSTACK_CALL;

	D3D11_RESOURCE_DIMENSION dimension;
	m_texture->GetType(&dimension);

	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		com_ptr<ID3D11Texture1D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture1D>(mst::initialize(tex)));
		D3D11_TEXTURE1D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::ConvertBindFlags(desc.BindFlags);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		com_ptr<ID3D11Texture2D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture2D>(mst::initialize(tex)));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::ConvertBindFlags(desc.BindFlags);
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		com_ptr<ID3D11Texture3D> tex;
		CHECK_HRESULT(m_texture->QueryInterface<ID3D11Texture3D>(mst::initialize(tex)));
		D3D11_TEXTURE3D_DESC desc;
		tex->GetDesc(&desc);

		return EnumConverter::ConvertBindFlags(desc.BindFlags);
	}
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
	default:
		Common::FatalError("Unknown resource dimension");
	}
}
