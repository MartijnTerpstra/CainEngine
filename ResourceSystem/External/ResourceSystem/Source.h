#pragma once

namespace CainEngine::ResourceSystem {

class Source
{
	COMMON_DECLARE_NON_COPY(Source);
public:
	// ctor & dtor
	Source();
	virtual ~Source();

public:
	// Main functionality

	uint32_t readUint();
	int32_t readInt();
	float readFloat();

	template<typename StructType>
	inline StructType readStruct();

	template<typename T>
	inline std::vector<T> readVector();

private:
	virtual bool read(void* data, size_t dataSize) = 0;
};

class FileSource final : public Source
{
public:
	// ctor & dtor
	FileSource(const std::string& filePath);
	~FileSource();

private:
	// Source overrides

	bool read(void* data, size_t dataSize) override;

private:
	// Member variables

	std::ifstream m_infile;
};

}
