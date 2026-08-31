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

	std::string toString() const
	{
		return mst::to_printf_string("%u.%u.%u", major(), minor(), revision());
	}

	uint32_t major() const
	{
		return data >> 24;
	}

	uint32_t minor() const
	{
		return (data >> 16) & 0xFF;
	}

	uint32_t revision() const
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
	: m_renderer(renderer.implementation())
	, m_factory(renderer.implementation()->getFactory())
{ }

Factory::~Factory()
{ }

std::pair<int32_t, API::ITexture*> Factory::loadTexture(Common::Source& source)
{
	auto header = source.readStruct<ImageHeader>();

	if(header.magicNumber != Common::packChars('c', 'e', 't', 'f'))
	{
		Common::error("Invalid file format");
		return { -1, nullptr };
	}

	if(header.version.major() != g_currentVerion.major())
	{
		Common::error("'Major version mismatch: %s != %s", header.version.toString(),
			g_currentVerion.toString());
		return { -1, nullptr };
	}

	if(header.version.minor() < g_currentVerion.minor())
	{
		Common::error("'Minor version mismatch: %s != %s", header.version.toString(),
			g_currentVerion.toString());
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
		subresources[i].pitch = source.readUint();

		subresourceDatas.push_back(source.readVector<uint8_t>());

		subresources[i].size = (uint32_t)subresourceDatas.back().size();
		subresources[i].pixels = subresourceDatas.back().data();
	}

	return m_factory->createTexture(m_renderer, header.type, size, subresources, header.format,
		API::BindFlags::ShaderResource, API::Usage::Constant, arraySize, header.mipCount);
}