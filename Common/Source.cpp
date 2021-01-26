#include "Precomp.h"

using namespace Common;

Source::Source()
{
}

Source::~Source()
{
}

uint32_t Source::ReadUint()
{
	uint32_t retval;
	if (!Read(&retval, sizeof(retval)))
	{
		Common::Error("Failed to read uint32_t value");
		return 0;
	}
	return retval;
}

int32_t Source::ReadInt()
{
	int32_t retval;
	if (!Read(&retval, sizeof(retval)))
	{
		Common::Error("Failed to read int32_t value");
		return 0;
	}
	return retval;
}

float Source::ReadFloat()
{
	float retval;
	if (!Read(&retval, sizeof(retval)))
	{
		Common::Error("Failed to read float value");
		return 0;
	}
	return retval;
}

FileSource::FileSource(const string& filePath)
	: m_infile(filePath, std::ios::binary)
{
	if (m_infile.fail())
	{
		Common::Error("Failure to open file: %s", filePath);
	}
}

FileSource::~FileSource()
{
}

bool FileSource::Read(void* data, size_t dataSize)
{
	m_infile.read(reinterpret_cast<char*>(data), dataSize);

	return !m_infile.fail();
}