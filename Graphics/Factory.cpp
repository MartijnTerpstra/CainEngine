#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

struct Version
{
	inline constexpr Version(uint8_t major, uint8_t minor, uint16_t revision)
		: data(((uint32_t)major << 24) + ((uint32_t)minor << 16) + revision)
	{ }

	inline Version()
		: data(0)
	{ }

	std::string ToString() const
	{
		return mst::to_printf_string("%u.%u.%u", Major(), Minor(), Revision());
	}

	uint32_t Major() const
	{
		return data >> 24;
	}

	uint32_t Minor() const
	{
		return (data >> 16) & 0xFF;
	}

	uint32_t Revision() const
	{
		return data & 0xFFFF;
	}

	const uint32_t data;
};

struct ImageHeader
{
	uint32_t magicNumber;
	Version version;
	uint32_t width, height, depthOrArraySize;
	uint32_t mipCount;
	PixelFormat format;
	ResourceType type;
};

struct SubresourceHeader
{
	uint32_t pitch;
	uint32_t size;
};

constexpr static const Version g_currentVerion{ 1, 0, 0 };

Factory::Factory(Renderer& renderer)
	: m_renderer(renderer.Implementation())
	, m_factory(renderer.Implementation()->GetFactory())
{ }

Factory::~Factory()
{ }

std::pair<int32_t, API::ITexture*> Factory::LoadTexture(Common::Source& source)
{
	auto header = source.ReadStruct<ImageHeader>();

	if(header.magicNumber != Common::PackChars('c', 'e', 't', 'f'))
	{
		Common::Error("Invalid file format");
		return { -1, nullptr };
	}

	if(header.version.Major() != g_currentVerion.Major())
	{
		Common::Error("'Major version mismatch: %s != %s", header.version.ToString(),
			g_currentVerion.ToString());
		return { -1, nullptr };
	}

	if(header.version.Minor() < g_currentVerion.Minor())
	{
		Common::Error("'Minor version mismatch: %s != %s", header.version.ToString(),
			g_currentVerion.ToString());
		return { -1, nullptr };
	}

	uint3 size = { header.width, header.height, 1 };
	uint32_t arraySize = header.depthOrArraySize;

	if(header.type == ResourceType::Tex3D)
	{
		std::swap(size.z, arraySize);
	}

	uint32_t subresourceCount = arraySize * header.mipCount;

	if(header.type == ResourceType::TexCube)
	{
		subresourceCount *= 6;
	}

	inlined_vector<std::vector<uint8_t>, 16> subresourceDatas;
	subresourceDatas.reserve(subresourceCount);

	inlined_vector<API::PixelData, 16> subresources(subresourceCount);

	for(size_t i = 0; i < subresourceCount; ++i)
	{
		subresources[i].pitch = source.ReadUint();

		subresourceDatas.push_back(source.ReadVector<uint8_t>());

		subresources[i].size = (uint32_t)subresourceDatas.back().size();
		subresources[i].pixels = subresourceDatas.back().data();
	}

	return m_factory->CreateTexture(m_renderer, header.type, size, subresources, header.format,
		API::BindFlags::ShaderResource, API::Usage::Constant, arraySize, header.mipCount);
}