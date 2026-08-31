#include <Common.h>

#include <gtest/gtest.h>

#include <cstring>
#include <filesystem>
#include <fstream>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

template<typename T>
std::vector<uint8_t> toBytes(const T& value)
{
	std::vector<uint8_t> bytes(sizeof(T));
	std::memcpy(bytes.data(), &value, sizeof(T));
	return bytes;
}

std::vector<uint8_t> concat(std::initializer_list<std::vector<uint8_t>> chunks)
{
	std::vector<uint8_t> result;
	for(auto& chunk : chunks)
		result.insert(result.end(), chunk.begin(), chunk.end());
	return result;
}

// A Source backed by an in-memory buffer, so read behavior (including
// truncated/failed reads) can be tested without touching disk.
class MemorySource : public Source
{
public:
	explicit MemorySource(std::vector<uint8_t> bytes)
		: m_bytes(std::move(bytes))
	{ }

private:
	bool read(void* data, size_t dataSize) override
	{
		if(m_offset + dataSize > m_bytes.size())
			return false;

		std::memcpy(data, m_bytes.data() + m_offset, dataSize);
		m_offset += dataSize;
		return true;
	}

private:
	std::vector<uint8_t> m_bytes;
	size_t m_offset = 0;
};

struct PodStruct
{
	int32_t a;
	float b;

	friend bool operator==(const PodStruct& l, const PodStruct& r)
	{
		return l.a == r.a && l.b == r.b;
	}
};

} // namespace

// -- ReadUint / ReadInt / ReadFloat --------------------------------------------

TEST(Source, ReadUintReturnsWrittenValue)
{
	MemorySource source(toBytes(uint32_t(0xDEADBEEFu)));

	EXPECT_EQ(0xDEADBEEFu, source.readUint());
}

TEST(Source, ReadIntReturnsWrittenNegativeValue)
{
	MemorySource source(toBytes(int32_t(-12345)));

	EXPECT_EQ(-12345, source.readInt());
}

TEST(Source, ReadFloatReturnsWrittenValue)
{
	MemorySource source(toBytes(3.5f));

	EXPECT_FLOAT_EQ(3.5f, source.readFloat());
}

TEST(Source, ReadUintOnEmptySourceReturnsZeroWithoutCrashing)
{
	MemorySource source({});

	EXPECT_EQ(0u, source.readUint());
}

TEST(Source, SuccessiveReadsAdvanceThroughTheBuffer)
{
	MemorySource source(
		concat({ toBytes(uint32_t(1)), toBytes(uint32_t(2)), toBytes(uint32_t(3)) }));

	EXPECT_EQ(1u, source.readUint());
	EXPECT_EQ(2u, source.readUint());
	EXPECT_EQ(3u, source.readUint());
}

// -- ReadStruct -----------------------------------------------------------------

TEST(Source, ReadStructReturnsWrittenFields)
{
	PodStruct value{ 42, 1.25f };
	MemorySource source(toBytes(value));

	EXPECT_EQ(value, source.readStruct<PodStruct>());
}

TEST(Source, ReadStructOnTruncatedSourceReturnsZeroedStruct)
{
	MemorySource source(toBytes(int32_t(1))); // too short for PodStruct

	EXPECT_EQ((PodStruct{ 0, 0.0f }), source.readStruct<PodStruct>());
}

// -- ReadVector -------------------------------------------------------------------

TEST(Source, ReadVectorRoundTripsElements)
{
	auto bytes = concat(
		{ toBytes(uint32_t(3)), toBytes(int32_t(10)), toBytes(int32_t(20)), toBytes(int32_t(30)) });
	MemorySource source(std::move(bytes));

	EXPECT_EQ((std::vector<int32_t>{ 10, 20, 30 }), source.readVector<int32_t>());
}

TEST(Source, ReadVectorWithZeroSizeReturnsEmptyVector)
{
	MemorySource source(toBytes(uint32_t(0)));

	EXPECT_TRUE(source.readVector<int32_t>().empty());
}

TEST(Source, ReadVectorFailsGracefullyWhenTruncated)
{
	// Header claims 5 elements, but only one is actually present.
	auto bytes = concat({ toBytes(uint32_t(5)), toBytes(int32_t(1)) });
	MemorySource source(std::move(bytes));

	EXPECT_TRUE(source.readVector<int32_t>().empty());
}

// -- FileSource -------------------------------------------------------------------

namespace {

class FileSourceTest : public ::testing::Test
{
protected:
	std::filesystem::path writeTempFile(const std::vector<uint8_t>& bytes)
	{
		auto path = std::filesystem::temp_directory_path() /
					("CainEngine_CommonTests_Source_" + std::to_string(m_fileCounter++) + ".bin");

		std::ofstream file(path, std::ios::binary);
		file.write(reinterpret_cast<const char*>(bytes.data()), (std::streamsize)bytes.size());
		file.close();

		m_writtenFiles.push_back(path);
		return path;
	}

	void TearDown() override
	{
		for(auto& path : m_writtenFiles)
			std::filesystem::remove(path);
	}

private:
	std::vector<std::filesystem::path> m_writtenFiles;
	int m_fileCounter = 0;
};

} // namespace

TEST_F(FileSourceTest, ReadsBackValuesWrittenToDisk)
{
	auto path = writeTempFile(concat({ toBytes(uint32_t(7)), toBytes(int32_t(-3)) }));

	FileSource source(path.string());

	EXPECT_EQ(7u, source.readUint());
	EXPECT_EQ(-3, source.readInt());
}

TEST_F(FileSourceTest, MissingFileFailsGracefullyOnRead)
{
	FileSource source(
		(std::filesystem::temp_directory_path() / "CainEngine_CommonTests_Source_DoesNotExist.bin")
			.string());

	// Construction logs an error (file failed to open) but doesn't throw or
	// abort; reads from it should degrade gracefully to the same "failed
	// read" behavior as a truncated in-memory source.
	EXPECT_EQ(0u, source.readUint());
}
