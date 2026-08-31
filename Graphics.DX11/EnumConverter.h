#pragma once

namespace CainEngine::Graphics::DX11 {

class EnumConverter
{
public:
	static PixelFormat convert(DXGI_FORMAT format);

	static DXGI_FORMAT convert(PixelFormat format);

	static D3D11_FILTER convert(TextureSamplingMode sampler, UINT& maxAnisotropy);

	static D3D11_TEXTURE_ADDRESS_MODE convert(TextureAddressMode address);

	static API::ShaderVariableType convert(D3D_REGISTER_COMPONENT_TYPE type);

	static DXGI_FORMAT convert(API::ShaderVariableType type, uint32_t elementCount);

	static D3D11_CULL_MODE convert(CullingMode culling);

	static D3D11_FILL_MODE convert(FillingMode filling);

	static D3D11_PRIMITIVE_TOPOLOGY convert(PrimitiveTopology topology);

	static UINT convert(flag<API::BindFlags> bindFlags);

	static flag<API::BindFlags> convertBindFlags(UINT bindFlags);

	static API::Usage convert(D3D11_USAGE usage);

	struct UsageConvertResult
	{
		D3D11_USAGE usage;
		UINT cpuAccess;
	};
	static UsageConvertResult convert(API::Usage usage);

	static API::ShaderSemanticName convert(const char* semantic);

	static const char* convert(API::ShaderSemanticName semantic);
};

} // namespace CainEngine::Graphics::DX11