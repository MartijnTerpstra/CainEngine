#include <Common.h>

#include <gtest/gtest.h>

#include <cstring>
#include <filesystem>
#include <fstream>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

template<typename T>
std::vector<byte> ToBytes(const T& value)
{
	std::vector<byte> bytes(sizeof(T));
	std::memcpy(bytes.data(), &value, sizeof(T));
	return bytes;
}

std::vector<byte> Concat(std::initializer_list<std::vector<byte>> chunks)
{
	std::vector<byte> result;
	for(auto& chunk : chunks)
		result.insert(result.end(), chunk.begin(), chunk.end());
	return result;
}

// A Source backed by an in-memory buffer, so read behavior (including
// truncated/failed reads) can be tested without touching disk.
class MemorySource : public Source
{
public:
	explicit MemorySource(std::vector<byte> bytes)
		: m_bytes(std::move(bytes))
	{ }

private:
	bool Read(void* data, size_t dataSize) override
	{
		if(m_offset + dataSize > m_bytes.size())
			return false;

		std::memcpy(data, m_bytes.data() + m_offset, dataSize);
		m_offset += dataSize;
		return true;
	}

private:
	std::vector<byte> m_bytes;
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
	MemorySource source(ToBytes(uint32_t(0xDEADBEEFu)));

	EXPECT_EQ(0xDEADBEEFu, source.ReadUint());
}

TEST(Source, ReadIntReturnsWrittenNegativeValue)
{
	MemorySource source(ToBytes(int32_t(-12345)));

	EXPECT_EQ(-12345, source.ReadInt());
}

TEST(Source, ReadFloatReturnsWrittenValue)
{
	MemorySource source(ToBytes(3.5f));

	EXPECT_FLOAT_EQ(3.5f, source.ReadFloat());
}

TEST(Source, ReadUintOnEmptySourceReturnsZeroWithoutCrashing)
{
	MemorySource source({});

	EXPECT_EQ(0u, source.ReadUint());
}

TEST(Source, SuccessiveReadsAdvanceThroughTheBuffer)
{
	MemorySource source(Concat({ToBytes(uint32_t(1)), ToBytes(uint32_t(2)), ToBytes(uint32_t(3))}));

	EXPECT_EQ(1u, source.ReadUint());
	EXPECT_EQ(2u, source.ReadUint());
	EXPECT_EQ(3u, source.ReadUint());
}

// -- ReadStruct -----------------------------------------------------------------

TEST(Source, ReadStructReturnsWrittenFields)
{
	PodStruct value{42, 1.25f};
	MemorySource source(ToBytes(value));

	EXPECT_EQ(value, source.ReadStruct<PodStruct>());
}

TEST(Source, ReadStructOnTruncatedSourceReturnsZeroedStruct)
{
	MemorySource source(ToBytes(int32_t(1))); // too short for PodStruct

	EXPECT_EQ((PodStruct{0, 0.0f}), source.ReadStruct<PodStruct>());
}

// -- ReadVector -------------------------------------------------------------------

TEST(Source, ReadVectorRoundTripsElements)
{
	auto bytes = Concat(
		{ToBytes(uint32_t(3)), ToBytes(int32_t(10)), ToBytes(int32_t(20)), ToBytes(int32_t(30))});
	MemorySource source(std::move(bytes));

	EXPECT_EQ((std::vector<int32_t>{10, 20, 30}), source.ReadVector<int32_t>());
}

TEST(Source, ReadVectorWithZeroSizeReturnsEmptyVector)
{
	MemorySource source(ToBytes(uint32_t(0)));

	EXPECT_TRUE(source.ReadVector<int32_t>().empty());
}

TEST(Source, ReadVectorFailsGracefullyWhenTruncated)
{
	// Header claims 5 elements, but only one is actually present.
	auto bytes = Concat({ToBytes(uint32_t(5)), ToBytes(int32_t(1))});
	MemorySource source(std::move(bytes));

	EXPECT_TRUE(source.ReadVector<int32_t>().empty());
}

// -- FileSource -------------------------------------------------------------------

namespace {

class FileSourceTest : public ::testing::Test
{
protected:
	std::filesystem::path WriteTempFile(const std::vector<byte>& bytes)
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
	auto path = WriteTempFile(Concat({ToBytes(uint32_t(7)), ToBytes(int32_t(-3))}));

	FileSource source(path.string());

	EXPECT_EQ(7u, source.ReadUint());
	EXPECT_EQ(-3, source.ReadInt());
}

TEST_F(FileSourceTest, MissingFileFailsGracefullyOnRead)
{
	FileSource source((std::filesystem::temp_directory_path() /
		"CainEngine_CommonTests_Source_DoesNotExist.bin")
			.string());

	// Construction logs an error (file failed to open) but doesn't throw or
	// abort; reads from it should degrade gracefully to the same "failed
	// read" behavior as a truncated in-memory source.
	EXPECT_EQ(0u, source.ReadUint());
}
