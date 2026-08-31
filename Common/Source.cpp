#include "Precomp.h"

using namespace CainEngine::Common;

Source::Source()
{ }

Source::~Source()
{ }

uint32_t Source::readUint()
{
	uint32_t retval;
	if(!read(&retval, sizeof(retval)))
	{
		Common::error("Failed to read uint32_t value");
		return 0;
	}
	return retval;
}

int32_t Source::readInt()
{
	int32_t retval;
	if(!read(&retval, sizeof(retval)))
	{
		Common::error("Failed to read int32_t value");
		return 0;
	}
	return retval;
}

float Source::readFloat()
{
	float retval;
	if(!read(&retval, sizeof(retval)))
	{
		Common::error("Failed to read float value");
		return 0;
	}
	return retval;
}

FileSource::FileSource(const std::string& filePath)
	: m_infile(filePath, std::ios::binary)
{
	if(m_infile.fail())
	{
		Common::error("Failure to open file: %s", filePath);
	}
}

FileSource::~FileSource()
{ }

bool FileSource::read(void* data, size_t dataSize)
{
	m_infile.read(reinterpret_cast<char*>(data), dataSize);

	return !m_infile.fail();
}