#pragma once

namespace CainEngine::Common {

template<typename StructType>
inline StructType Source::readStruct()
{
	StructType retval;
	if(!read(&retval, sizeof(StructType)))
	{
		Common::error("Unable to read struct");
		return StructType{};
	}
	return retval;
}

template<typename T>
inline std::vector<T> Source::readVector()
{
	uint32_t size = readUint();

	std::vector<T> retval(size);
	if(size != 0)
	{
		if(!read(retval.data(), sizeof(T) * size))
		{
			Common::error("Unable to read vector<T>");
			return {};
		}
	}

	return retval;
}

} // namespace CainEngine::Common