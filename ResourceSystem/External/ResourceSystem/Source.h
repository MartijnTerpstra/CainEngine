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

	uint32_t ReadUint();
	int32_t ReadInt();
	float ReadFloat();

	template<typename StructType>
	inline StructType ReadStruct();

	template<typename T>
	inline vector<T> ReadVector();

private:
	virtual bool Read(void* data, size_t dataSize) = 0;
};

class FileSource final : public Source
{
public:
	// ctor & dtor
	FileSource(const string& filePath);
	~FileSource();

private:
	// Source overrides

	bool Read(void* data, size_t dataSize) override;

private:
	// Member variables

	std::ifstream m_infile;
};

}