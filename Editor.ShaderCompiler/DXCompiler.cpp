#include "Precomp.h"

#include "ICompiler.h"
#include "DXCompiler.h"

#include <d3dcompiler.h>
#include <mcom_ptr.h>
using mst::com_ptr;

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Editor::ShaderCompiler;

// CainEngine Compiled HLSL
constexpr uint32_t DX11_ID = Common::PackChars('d', 'x', '1', '1');
constexpr uint32_t DX12_ID = Common::PackChars('d', 'x', '1', '2');

class DXCompiler : public ICompiler
{
public:
	// ctor & dtor

	DXCompiler(const char* rendererType, uint32_t rendererID, const char* featureLevel);
	~DXCompiler();

public:
	// ICompiler overrides

	API::CompiledShaderData Compile(const char* sourceDirectory,
		const char* filePath,
		ShaderType shaderType,
		const char* entryPoint,
		const vector<ShaderDefine>& defines,
		bool optimize) const override;

	string RendererType() const override;

	uint32_t RendererID() const override;

private:

	static API::ShaderVariableType Convert(D3D_REGISTER_COMPONENT_TYPE type);
	static API::ShaderSemanticName Convert(const char* semantic);

private:
	// Member variables

	HMODULE m_d3dCompiler;
	decltype(&D3DCompileFromFile) m_d3dCompileFunc;
	decltype(&D3DReflect) m_d3dReflectFunc;
	const char* const m_hlslFeatureLevel;
	const string m_type;
	const uint32_t m_rendererID;
};

unique_ptr<ICompiler> CainEngine::Editor::ShaderCompiler::CreateDX11Compiler()
{
	return make_unique<DXCompiler>("DX11", DX11_ID, "_5_0");
}

unique_ptr<ICompiler> CainEngine::Editor::ShaderCompiler::CreateDX12Compiler()
{
	return make_unique<DXCompiler>("DX12", DX12_ID, "_5_1");
}

DXCompiler::DXCompiler(const char* rendererType, uint32_t rendererID, const char* featureLevel)
	: m_type(rendererType),
	m_rendererID(rendererID),
	m_hlslFeatureLevel(featureLevel)
{
	m_d3dCompiler = LoadLibraryA(D3DCOMPILER_DLL_A);

	if (!m_d3dCompiler)
		Common::FatalError("LoadLibraryA failed, ErrorCode: %u", GetLastError());

	m_d3dCompileFunc = reinterpret_cast<decltype(m_d3dCompileFunc)>(GetProcAddress(m_d3dCompiler, "D3DCompileFromFile"));
	m_d3dReflectFunc = reinterpret_cast<decltype(m_d3dReflectFunc)>(GetProcAddress(m_d3dCompiler, "D3DReflect"));
}

DXCompiler::~DXCompiler()
{
	FreeLibrary(m_d3dCompiler);
}

API::CompiledShaderData DXCompiler::Compile(const char* sourceDirectory,
	const char* filePath,
	ShaderType shaderType,
	const char* entryPoint,
	const vector<ShaderDefine>& defines,
	bool optimize) const
{
	COMMON_CALLSTACK_CALL;

	std::wstring wpath;
	{
		string path = string(sourceDirectory) + "\\" + filePath;

		std::ifstream local(path);
		if (!local)
		{
			path = filePath;
		}

		wpath.resize(path.length());

		mbstowcs(&wpath[0], path.c_str(), wpath.length());
	}

	DWORD flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

	if (optimize)
	{
		flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
	}
	else
	{
		flags |= D3DCOMPILE_DEBUG;
	}

	std::string target;
	switch (shaderType)
	{
	case ShaderType::Vertex:
		target = "vs";
		break;
	case ShaderType::Pixel:
		target = "ps";
		break;
	case ShaderType::Compute:
		target = "cs";
		break;
	case ShaderType::Geometry:
		target = "gs";
		break;
	case ShaderType::Hull:
		target = "hs";
		break;
	case ShaderType::Domain:
		target = "ds";
		break;
	default:
		break;
	}
	target.append(m_hlslFeatureLevel);

	vector<D3D_SHADER_MACRO> macros(defines.size() + 2);

	string rendererDefine = "IS_" + m_type + "_RENDERER";

	macros[0].Name = rendererDefine.c_str();
	macros[0].Definition = "1";

	for (size_t i = 0; i < defines.size(); ++i)
	{
		macros[i + 1].Name = defines[i].key.c_str();
		macros[i + 1].Definition = defines[i].value.c_str();
	}

	macros.back().Name = nullptr;
	macros.back().Definition = nullptr;

	com_ptr<ID3DBlob> byteCode, errorMesg;
	auto hr = m_d3dCompileFunc(wpath.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target.c_str(), flags, 0, mst::initialize(byteCode), mst::initialize(errorMesg));

	if (FAILED(hr))
	{
		string error = (const char*)errorMesg->GetBufferPointer();

		size_t fromIndex = 0;
		while (fromIndex < error.length())
		{
			size_t newlineIndex = error.find('\n', fromIndex);

			if (newlineIndex == string::npos)
				break;

			Common::Error(error.substr(fromIndex, newlineIndex - fromIndex).c_str());

			fromIndex = newlineIndex + 1;
		}

		return {};
	}

	API::CompiledShaderData r;

	r.byteCode.resize(byteCode->GetBufferSize());
	memcpy(r.byteCode.data(), byteCode->GetBufferPointer(), r.byteCode.size());

	byteCode.reset();

	com_ptr<ID3D12ShaderReflection> reflection;
	if (FAILED(m_d3dReflectFunc(r.byteCode.data(), r.byteCode.size(), MST_IID_PPV_ARGS(reflection))))
	{
		Common::Error("Unable to reflect shader");
		return {};
	}

	D3D12_SHADER_DESC desc;
	reflection->GetDesc(&desc);

	for (uint i = 0; i < desc.InputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC inputDesc;
		reflection->GetInputParameterDesc(i, &inputDesc);

		API::ShaderRegisterInfo sem;

		sem.registerSlot = inputDesc.Register;
		sem.variableType = Convert(inputDesc.ComponentType);

		sem.semanticName = Convert(inputDesc.SemanticName);
		sem.semanticIndex = inputDesc.SemanticIndex;

		int idx = 0;
		sem.variableElementCount = 0;
		while (inputDesc.Mask & (1 << idx++))
		{
			++sem.variableElementCount;
		}

		r.inputRegisters.push_back(move(sem));
	}

	std::sort(r.inputRegisters.begin(), r.inputRegisters.end(), [](const API::ShaderRegisterInfo& l, const API::ShaderRegisterInfo& r)
	{
		return l.registerSlot < r.registerSlot;
	});

	for (uint i = 0; i < desc.OutputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC inputDesc;
		reflection->GetOutputParameterDesc(i, &inputDesc);

		API::ShaderRegisterInfo sem;

		sem.registerSlot = inputDesc.Register;
		sem.variableType = Convert(inputDesc.ComponentType);

		int idx = 0;
		sem.variableElementCount = 0;
		while (inputDesc.Mask & (1 << idx++))
		{
			++sem.variableElementCount;
		}

		r.outputRegisters.push_back(move(sem));
	}

	for (uint i = 0; i < desc.BoundResources; ++i)
	{

		D3D12_SHADER_INPUT_BIND_DESC inputDesc;
		reflection->GetResourceBindingDesc(i, &inputDesc);

		ShaderTextureInfo tex;
		ShaderBufferInfo buf;

		switch (inputDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			switch (inputDesc.Dimension)
			{
			case D3D_SRV_DIMENSION_TEXTURE1D:
				tex.type = ResourceType::Tex1D;
				break;
			case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
				tex.type = ResourceType::Tex1D;
				tex.IsArray = true;
				break;
			case D3D_SRV_DIMENSION_TEXTURE2D:
				tex.type = ResourceType::Tex2D;
				break;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
				tex.type = ResourceType::Tex2D;
				tex.IsArray = true;
				break;
			case D3D_SRV_DIMENSION_TEXTURE3D:
				tex.type = ResourceType::Tex3D;
				break;
			case D3D_SRV_DIMENSION_TEXTURECUBE:
				tex.type = ResourceType::TexCube;
				break;
			case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:
				tex.type = ResourceType::TexCube;
				tex.IsArray = true;
				break;
			default:
				Common::FatalError("D3D_SRV_DIMENSION corrupted");
			}
			tex.nameHash = mst::hash64(inputDesc.Name);
			tex.slot = inputDesc.BindPoint;
			r.textures.push_back(move(tex));
			continue;
		case D3D_SIT_CBUFFER:
			buf.type = ResourceType::ConstantBuffer;
			{
				auto cbuffer = reflection->GetConstantBufferByName(inputDesc.Name);

				D3D12_SHADER_BUFFER_DESC cbufferDesc;
				cbuffer->GetDesc(&cbufferDesc);
				buf.dataSize = cbufferDesc.Size;
			}
			break;
		case D3D_SIT_BYTEADDRESS:
			buf.type = ResourceType::ByteAddressBuffer;
			break;
		case D3D_SIT_STRUCTURED:
			buf.type = ResourceType::StructuredBuffer;
			break;
		case D3D_SIT_SAMPLER:
			continue;
		default:
			Common::FatalError("D3D_SHADER_INPUT_TYPE corrupted");
			break;
		}

		buf.nameHash = mst::hash64(inputDesc.Name);
		buf.slot = inputDesc.BindPoint;

		r.buffers.push_back(move(buf));
	}

	return r;
}

string DXCompiler::RendererType() const
{
	return m_type;
}

uint32_t DXCompiler::RendererID() const
{
	return m_rendererID;
}

API::ShaderVariableType DXCompiler::Convert(D3D_REGISTER_COMPONENT_TYPE type)
{
	COMMON_CALLSTACK_CALL;

	switch (type)
	{
	case D3D_REGISTER_COMPONENT_UINT32:
		return API::ShaderVariableType::Uint32;
	case D3D_REGISTER_COMPONENT_SINT32:
		return API::ShaderVariableType::Int32;
	case D3D_REGISTER_COMPONENT_FLOAT32:
		return API::ShaderVariableType::Float;
	case D3D_REGISTER_COMPONENT_UNKNOWN:
		Common::FatalError("D3D_REGISTER_COMPONENT_TYPE has a unknown value");
		return API::ShaderVariableType::Float;
	default:
		Common::FatalError("D3D_REGISTER_COMPONENT_TYPE has a corrupted value");
		return API::ShaderVariableType::Float;
	}
}

API::ShaderSemanticName DXCompiler::Convert(const char* semantic)
{
	COMMON_CALLSTACK_CALL;

	// Integral constant overflow because of compiletime hashing
	switch (mst::hash32(semantic))
	{
	case mst::compiletime::hash32("POSITION"):
	case mst::compiletime::hash32("SV_POSITION"):
		return API::ShaderSemanticName::Position;
	case mst::compiletime::hash32("TEXCOORD"):
		return API::ShaderSemanticName::Texcoord;
	case mst::compiletime::hash32("NORMAL"):
		return API::ShaderSemanticName::Normal;
	default:
		Common::FatalError("semantic name not found");
		return (API::ShaderSemanticName)-1;
	}
}