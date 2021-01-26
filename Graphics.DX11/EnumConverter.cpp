#include "Precomp.h"

#include "EnumConverter.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

PixelFormat EnumConverter::Convert(DXGI_FORMAT format)
{
	COMMON_CALLSTACK_CALL;

	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return PixelFormat::UnormRGBA8;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return PixelFormat::UnormBGRA8;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		return PixelFormat::UnormRGBA16;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return PixelFormat::FloatRGBA32;
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_A8_UNORM:
		return PixelFormat::Unorm8;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return PixelFormat::FloatRGBA16;
	default:
		Common::FatalError("DXGI_FORMAT corrupted");
		return (PixelFormat)-1;
	}
}

DXGI_FORMAT EnumConverter::Convert(PixelFormat format)
{
	COMMON_CALLSTACK_CALL;

	switch (format)
	{
	case PixelFormat::UnormRGBA8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PixelFormat::UnormBGRA8:
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case PixelFormat::UnormRGBA16:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case PixelFormat::FloatRGBA32:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case PixelFormat::Unorm8:
		return DXGI_FORMAT_R8_UNORM;
	case PixelFormat::BlockCompression1:
		return DXGI_FORMAT_BC1_UNORM;
	case PixelFormat::BlockCompression2:
		return DXGI_FORMAT_BC2_UNORM;
	case PixelFormat::BlockCompression3:
		return DXGI_FORMAT_BC3_UNORM;
	case PixelFormat::BlockCompression4:
		return DXGI_FORMAT_BC4_UNORM;
	case PixelFormat::BlockCompression5:
		return DXGI_FORMAT_BC5_UNORM;
	case PixelFormat::BlockCompression7:
		return DXGI_FORMAT_BC7_UNORM;
	case PixelFormat::DepthUnorm16:
		return DXGI_FORMAT_D16_UNORM;
	case PixelFormat::DepthUnorm24Stencil8:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case PixelFormat::DepthFloat32:
		return DXGI_FORMAT_D32_FLOAT;
	case PixelFormat::FloatRGBA16:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	default:
		Common::FatalError("Graphics::PixelFormat corrupted");
		return DXGI_FORMAT_UNKNOWN;
	}
}

D3D11_FILTER EnumConverter::Convert(TextureSamplingMode sampler, UINT& maxAnisotropy)
{
	COMMON_CALLSTACK_CALL;

	maxAnisotropy = 0;

	switch (sampler)
	{
	case Graphics::TextureSamplingMode::Nearest:
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case Graphics::TextureSamplingMode::Bilinear:
		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case Graphics::TextureSamplingMode::Trilinear:
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case Graphics::TextureSamplingMode::Anisotropic4x:
		maxAnisotropy = 4;
		return D3D11_FILTER_ANISOTROPIC;
	case Graphics::TextureSamplingMode::Anisotropic8x:
		maxAnisotropy = 8;
		return D3D11_FILTER_ANISOTROPIC;
		break;
	case Graphics::TextureSamplingMode::Anisotropic16x:
		maxAnisotropy = 16;
		return D3D11_FILTER_ANISOTROPIC;
		break;
	default:
		Common::FatalError("Graphics::TextureSamplingMode corrupted");
		maxAnisotropy = 0;
		return (D3D11_FILTER)-1;
	}
}

D3D11_TEXTURE_ADDRESS_MODE EnumConverter::Convert(TextureAddressMode address)
{
	COMMON_CALLSTACK_CALL;

	switch (address)
	{
	case Graphics::TextureAddressMode::Wrap:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case Graphics::TextureAddressMode::Clamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case Graphics::TextureAddressMode::Mirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	default:
		Common::FatalError("Graphics::TextureaddressMode corrupted");
		return (D3D11_TEXTURE_ADDRESS_MODE)-1;
	}
}

API::ShaderVariableType EnumConverter::Convert(D3D_REGISTER_COMPONENT_TYPE type)
{
	COMMON_CALLSTACK_CALL;

	using API::ShaderVariableType;

	switch (type)
	{
	case D3D_REGISTER_COMPONENT_UINT32:
		return ShaderVariableType::Uint32;
	case D3D_REGISTER_COMPONENT_SINT32:
		return ShaderVariableType::Int32;
	case D3D_REGISTER_COMPONENT_FLOAT32:
		return ShaderVariableType::Float;
	case D3D_REGISTER_COMPONENT_UNKNOWN:
		Common::FatalError("D3D_REGISTER_COMPONENT_TYPE has a unknown value");
		return ShaderVariableType::Float;
	default:
		Common::FatalError("D3D_REGISTER_COMPONENT_TYPE has a corrupted value");
		return ShaderVariableType::Float;
	}
}

DXGI_FORMAT EnumConverter::Convert(API::ShaderVariableType type, uint elementCount)
{
	COMMON_CALLSTACK_CALL;

	using API::ShaderVariableType;

	switch (type)
	{
	case ShaderVariableType::Float:
		switch (elementCount)
		{
		case 1:
			return DXGI_FORMAT_R32_FLOAT;
		case 2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case 3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case 4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			Common::FatalError("elementCount out of range");
			return (DXGI_FORMAT)-1;
		}
	case ShaderVariableType::Int32:
		switch (elementCount)
		{
		case 1:
			return DXGI_FORMAT_R32_SINT;
		case 2:
			return DXGI_FORMAT_R32G32_SINT;
		case 3:
			return DXGI_FORMAT_R32G32B32_SINT;
		case 4:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		default:
			Common::FatalError("elementCount out of range");
			return (DXGI_FORMAT)-1;
		}
	case ShaderVariableType::Uint32:
		switch (elementCount)
		{
		case 1:
			return DXGI_FORMAT_R32_UINT;
		case 2:
			return DXGI_FORMAT_R32G32_UINT;
		case 3:
			return DXGI_FORMAT_R32G32B32_UINT;
		case 4:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		default:
			Common::FatalError("elementCount out of range");
			return (DXGI_FORMAT)-1;
		}
	default:
		Common::FatalError("Graphics::ShaderVariableType corrupted");
		return (DXGI_FORMAT)-1;
	}
}

D3D11_CULL_MODE EnumConverter::Convert(CullingMode culling)
{
	COMMON_CALLSTACK_CALL;

	switch (culling)
	{
	case Graphics::CullingMode::None:
		return D3D11_CULL_NONE;
	case Graphics::CullingMode::Front:
		return D3D11_CULL_FRONT;
	case Graphics::CullingMode::Back:
		return D3D11_CULL_BACK;
	default:
		Common::FatalError("Graphics::CullingMode corrupted");
		return (D3D11_CULL_MODE)-1;
	}
}

D3D11_FILL_MODE EnumConverter::Convert(FillingMode filling)
{
	COMMON_CALLSTACK_CALL;

	switch (filling)
	{
	case Graphics::FillingMode::Wireframe:
		return D3D11_FILL_WIREFRAME;
	case Graphics::FillingMode::Solid:
		return D3D11_FILL_SOLID;
	default:
		Common::FatalError("Graphics::FillingMode corrupted");
		return (D3D11_FILL_MODE)-1;
	}
}

D3D11_PRIMITIVE_TOPOLOGY EnumConverter::Convert(PrimitiveTopology topology)
{
	COMMON_CALLSTACK_CALL;

	switch (topology)
	{
	case Graphics::PrimitiveTopology::TriangleList:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case Graphics::PrimitiveTopology::LineList:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case Graphics::PrimitiveTopology::PointList:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	default:
		Common::FatalError("Graphics::PrimitiveType corrupted");
		return (D3D11_PRIMITIVE_TOPOLOGY)-1;
	}
}

UINT EnumConverter::Convert(flag<API::BindFlags> bindFlags)
{
	UINT retval = 0;

	if (!bindFlags.is_enabled(API::BindFlags::ShaderResource))
		retval |= D3D11_BIND_SHADER_RESOURCE;

	if (!bindFlags.is_enabled(API::BindFlags::RenderTarget))
		retval |= D3D11_BIND_RENDER_TARGET;

	if (!bindFlags.is_enabled(API::BindFlags::DepthStencil))
		retval |= D3D11_BIND_DEPTH_STENCIL;

	return retval;
}

flag<API::BindFlags> EnumConverter::ConvertBindFlags(UINT bindFlags)
{
	flag<API::BindFlags> retval;

	if (bindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		retval.enable(API::BindFlags::ShaderResource);
	}

	if (bindFlags & D3D11_BIND_RENDER_TARGET)
	{
		retval.enable(API::BindFlags::RenderTarget);
	}

	if (bindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		retval.enable(API::BindFlags::DepthStencil);
	}
	return retval;
}

API::Usage EnumConverter::Convert(D3D11_USAGE usage)
{
	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		return API::Usage::GpuReadWrite;
	case D3D11_USAGE_IMMUTABLE:
		return API::Usage::Constant;
	case D3D11_USAGE_DYNAMIC:
		return API::Usage::Dynamic;
	case D3D11_USAGE_STAGING:
		return API::Usage::Staging;
	default:
		Common::FatalError("D3D11_USAGE corrupted");
		return (API::Usage)-1;
	}
}

EnumConverter::UsageConvertResult EnumConverter::Convert(API::Usage usage)
{
	switch (usage)
	{
	case CainEngine::Graphics::API::Usage::Constant:
		return { D3D11_USAGE_IMMUTABLE, 0 };
	case CainEngine::Graphics::API::Usage::GpuReadWrite:
		return { D3D11_USAGE_DEFAULT, 0 };
	case CainEngine::Graphics::API::Usage::Dynamic:
		return { D3D11_USAGE_DYNAMIC, D3D10_CPU_ACCESS_WRITE };
	case CainEngine::Graphics::API::Usage::Staging:
		return { D3D11_USAGE_STAGING, D3D10_CPU_ACCESS_READ };
	default:
		Common::FatalError("Graphics::API::Usage corrupted");
		return {};
	}
}

API::ShaderSemanticName EnumConverter::Convert(const char* semantic)
{
	COMMON_CALLSTACK_CALL;

	using API::ShaderSemanticName;

	// Integral constant overflow because of compiletime hashing
#pragma warning (push)
#pragma warning (disable : 4307)
	switch (mst::hash32(semantic))
	{
	case mst::compiletime::hash32("POSITION"):
	case mst::compiletime::hash32("SV_POSITION"):
		return ShaderSemanticName::Position;
	case mst::compiletime::hash32("TEXCOORD"):
		return ShaderSemanticName::Texcoord;
	case mst::compiletime::hash32("NORMAL"):
		return ShaderSemanticName::Normal;
	default:
		Common::FatalError("semantic name not found");
		return (ShaderSemanticName)-1;
	}
#pragma warning (pop)
}

const char* EnumConverter::Convert(API::ShaderSemanticName semantic)
{
	COMMON_CALLSTACK_CALL;

	using API::ShaderSemanticName;

	switch (semantic)
	{
	case ShaderSemanticName::Position:
		return "POSITION";
	case ShaderSemanticName::Texcoord:
		return "TEXCOORD";
	case ShaderSemanticName::Normal:
		return "NORMAL";
	default:
		Common::FatalError("ShaderSemanticName corrupted");
		return nullptr;
	}
}