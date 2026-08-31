#pragma once

namespace CainEngine::Common {

template<typename T>
inline std::optional<T> JsonParser::parse(
	const std::string& jsonFile, const JsonDeclaration<T>& declaration)
{
	m_failure = false;

	std::optional<T> retval = std::nullopt;

	parseImpl(jsonFile, [&](Details::JsonToken& rootToken) {
		T value;

		for(auto& action : declaration.m_parsingActions)
		{
			action(value, rootToken);
		}

		if(!m_failure)
			retval = value;
	});

	return retval;
}

template<typename T>
inline std::optional<std::vector<T>> JsonParser::parseArray(
	const std::string& jsonFile, const JsonDeclaration<T>& declaration)
{
	m_failure = false;

	std::optional<std::vector<T>> retval = std::nullopt;

	parseImpl(jsonFile, [&](std::vector<Details::JsonToken>& rootTokens) {
		std::vector<T> values(rootTokens.size());

		size_t index = 0;
		for(auto& rootToken : rootTokens)
		{
			auto& value = values[index++];
			for(auto& action : declaration.m_parsingActions)
			{
				action(value, rootToken);
			}
		}

		if(!m_failure)
			retval = values;
	});

	return retval;
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, bool T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member) = false;
			return;
		}

		(outValue.*member) = token.getBool(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, uint32_t T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.getUint32(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, float T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.getFloat(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, int32_t T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.getInt32(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::string T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getString(name);
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(
	const char* name, T2 T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		auto& memberObjectValue = (outValue.*memberObject);

		auto subToken = token.getObjectToken(name);

		for(auto& action : declaration.m_parsingActions)
		{
			action(memberObjectValue, subToken);
		}
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(
	const char* name, T2 T::* memberEnum, std::map<std::string, T2> conversionMap, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*memberEnum) = {};
			return;
		}

		auto str = token.getString(name);

		auto findIt = conversionMap.find(str);

		if(findIt == conversionMap.end())
		{
			error("%s: failed to map enum value: '%s'", name, str);
			(outValue.*memberEnum) = {};
			return;
		}

		(outValue.*memberEnum) = findIt->second;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::optional<bool> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getBool(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::optional<uint32_t> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getUint32(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::optional<float> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getFloat(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::optional<int32_t> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getInt32(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(const char* name, std::optional<std::string> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getString(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<T2> T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
		{
			(outValue.*memberObject) = T2();

			auto& memberObjectValue = *(outValue.*memberObject);

			auto subToken = token.getObjectToken(name);

			for(auto& action : declaration.m_parsingActions)
			{
				action(memberObjectValue, subToken);
			}
		}
		else
		{
			(outValue.*memberObject) = std::nullopt;
		}
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<T2> T::* memberEnum, std::map<std::string, T2> conversionMap)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
		{
			auto str = token.getString(name);

			auto findIt = conversionMap.find(str);

			if(findIt == conversionMap.end())
			{
				error("%s: failed to map enum value: '%s'", name, str);
				(outValue.*memberEnum) = {};
				return;
			}

			(outValue.*memberEnum) = findIt->second;
		}
		else
		{
			(outValue.*memberEnum) = std::nullopt;
		}
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::vector<bool> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getBools(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::vector<uint32_t> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getUint32s(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::vector<float> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getFloats(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::vector<int32_t> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getInt32s(name);
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::vector<std::string> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(optional && !token.hasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.getStrings(name);
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(const char* name, std::vector<T2> T::* memberObjects,
	JsonDeclaration<T2> declaration, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		(outValue.*memberObjects).clear();

		if(optional && !token.hasValue(name))
			return;

		auto& memberObjectValue = (outValue.*memberObjects);

		auto subTokens = token.getObjectTokens(name);

		for(auto& subToken : subTokens)
		{
			memberObjectValue.emplace_back();
			for(auto& action : declaration.m_parsingActions)
			{
				action(memberObjectValue.back(), subToken);
			}
		}
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(const char* name, std::vector<T2> T::* memberEnums,
	std::map<std::string, T2> conversionMap, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		(outValue.*memberEnums).clear();

		if(optional && !token.hasValue(name))
			return;

		auto strs = token.getStrings(name);

		auto& enumList = (outValue.*memberEnums);
		enumList.reserve(strs.size());

		for(auto& str : strs)
		{
			auto findIt = conversionMap.find(str);

			if(findIt == conversionMap.end())
			{
				error("%s: failed to map enum value: '%s'", name, str);
				(outValue.*memberEnums).clear();
				return;
			}

			enumList.push_back(findIt->second);
		}
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<std::vector<uint32_t>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getUint32s(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<std::vector<float>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getFloats(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<std::vector<int32_t>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getInt32s(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
inline void JsonDeclaration<T>::addMember(
	const char* name, std::optional<std::vector<std::string>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
			(outValue.*member) = token.getStrings(name);
		else
			(outValue.*member) = std::nullopt;
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(const char* name,
	std::optional<std::vector<T2>> T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
		{
			(outValue.*memberObject) = std::vector<T2>();

			auto& memberObjectValue = *(outValue.*memberObject);

			auto subTokens = token.getObjectTokens(name);
			memberObjectValue.reserve(subTokens.size());

			for(auto& subToken : subTokens)
			{
				memberObjectValue.emplace_back();
				for(auto& action : declaration.m_parsingActions)
				{
					action(memberObjectValue.back(), subToken);
				}
			}
		}
		else
		{
			(outValue.*memberObject) = std::nullopt;
		}
	});
}

template<typename T>
template<typename T2>
inline void JsonDeclaration<T>::addMember(const char* name,
	std::optional<std::vector<T2>> T::* memberEnums, std::map<std::string, T2> conversionMap)
{
	m_parsingActions.push_back([=](T& outValue, Details::JsonToken& token) {
		if(token.hasValue(name))
		{
			auto strs = token.getStrings(name);

			(outValue.*memberEnums) = std::vector<T2>();

			auto& enumList = *(outValue.*memberEnums);
			enumList.reserve(strs.size());

			for(auto& str : strs)
			{
				auto findIt = conversionMap.find(str);

				if(findIt == conversionMap.end())
				{
					error("%s: failed to map enum value: '%s'", name, str);
					(outValue.*memberEnums).clear();
					return;
				}

				enumList.push_back(findIt->second);
			}
		}
		else
		{
			(outValue.*memberEnums) = std::nullopt;
		}
	});
}

} // namespace CainEngine::Common