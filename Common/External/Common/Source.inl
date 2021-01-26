#pragma once

template<typename StructType>
inline StructType Common::Source::ReadStruct()
{
	StructType retval;
	if (!Read(&retval, sizeof(StructType)))
	{
		Common::Error("Unable to read struct");
		return StructType{};
	}
	return retval;
}

template<typename T>
inline vector<T> Common::Source::ReadVector()
{
	uint32_t size = ReadUint();

	vector<T> retval(size);
	if (size != 0)
	{
		if (!Read(retval.data(), sizeof(T) * size))
		{
			Common::Error("Unable to read vector<T>");
			return {};
		}
	}

	return retval;
}