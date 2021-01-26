#pragma once

namespace CainEngine::Graphics::DX11 {

class EnumConverter
{
public:

	static PixelFormat Convert(DXGI_FORMAT format);

	static DXGI_FORMAT Convert(PixelFormat format);

	static D3D11_FILTER Convert(TextureSamplingMode sampler, UINT& maxAnisotropy);

	static D3D11_TEXTURE_ADDRESS_MODE Convert(TextureAddressMode address);

	static API::ShaderVariableType Convert(D3D_REGISTER_COMPONENT_TYPE type);

	static DXGI_FORMAT Convert(API::ShaderVariableType type, uint elementCount);

	static D3D11_CULL_MODE Convert(CullingMode culling);

	static D3D11_FILL_MODE Convert(FillingMode filling);

	static D3D11_PRIMITIVE_TOPOLOGY Convert(PrimitiveTopology topology);

	static UINT Convert(flag<API::BindFlags> bindFlags);

	static flag<API::BindFlags> ConvertBindFlags(UINT bindFlags);

	static API::Usage Convert(D3D11_USAGE usage);

	struct UsageConvertResult
	{
		D3D11_USAGE usage;
		UINT cpuAccess;
	};
	static UsageConvertResult Convert(API::Usage usage);

	static API::ShaderSemanticName Convert(const char* semantic);

	static const char* Convert(API::ShaderSemanticName semantic);

};

}