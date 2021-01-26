#pragma once

template<typename T>
inline optional<T> Common::JsonParser::Parse(const string& jsonFile, const JsonDeclaration<T>& declaration)
{
	m_failure = false;

	optional<T> retval = none;

	ParseImpl(jsonFile, [&](_Details::JsonToken& rootToken)
	{
		T value;

		for (auto& action : declaration.m_parsingActions)
		{
			action(value, rootToken);
		}

		if (!m_failure)
			retval = value;
	});

	return retval;
}

template<typename T>
inline optional<vector<T>> Common::JsonParser::ParseArray(const string& jsonFile, const JsonDeclaration<T>& declaration)
{
	m_failure = false;

	optional<vector<T>> retval = none;

	ParseImpl(jsonFile, [&](vector<_Details::JsonToken>& rootTokens)
	{
		vector<T> values(rootTokens.size());

		size_t index = 0;
		for (auto& rootToken : rootTokens)
		{
			auto& value = values[index++];
			for (auto& action : declaration.m_parsingActions)
			{
				action(value, rootToken);
			}
		}

		if (!m_failure)
			retval = values;
	});

	return retval;
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, bool T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member) = false;
			return;
		}

		(outValue.*member) = token.GetBool(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, uint32_t T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.GetUint32(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, float T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.GetFloat(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, int32_t T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member) = 0;
			return;
		}

		(outValue.*member) = token.GetInt32(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, string T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetString(name);
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, T2 T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		auto& memberObjectValue = (outValue.*memberObject);

		auto subToken = token.GetObjectToken(name);

		for (auto& action : declaration.m_parsingActions)
		{
			action(memberObjectValue, subToken);
		}
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, T2 T::* memberEnum, std::map<string, T2> conversionMap, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*memberEnum) = {};
			return;
		}

		auto str = token.GetString(name);

		auto findIt = conversionMap.find(str);

		if (findIt == conversionMap.end())
		{
			Common::Error("%s: failed to map enum value: '%s'", name, str);
			(outValue.*memberEnum) = {};
			return;
		}

		(outValue.*memberEnum) = findIt->second;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<bool> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetBool(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<uint32_t> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetUint32(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<float> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetFloat(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<int32_t> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetInt32(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<string> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetString(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<T2> T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
		{
			(outValue.*memberObject) = T2();

			auto& memberObjectValue = *(outValue.*memberObject);

			auto subToken = token.GetObjectToken(name);

			for (auto& action : declaration.m_parsingActions)
			{
				action(memberObjectValue, subToken);
			}
		}
		else
		{
			(outValue.*member) = none;
		}
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<T2> T::* memberEnum, std::map<string, T2> conversionMap)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
		{
			auto str = token.GetString(name);

			auto findIt = conversionMap.find(str);

			if (findIt == conversionMap.end())
			{
				Common::Error("%s: failed to map enum value: '%s'", name, str);
				(outValue.*memberEnum) = {};
				return;
			}

			(outValue.*memberEnum) = findIt->second;
		}
		else
		{
			(outValue.*member) = none;
		}
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<bool> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetBools(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<uint32_t> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetUint32s(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<float> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetFloats(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<int32_t> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetInt32s(name);
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<string> T::* member, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (optional && !token.HasValue(name))
		{
			(outValue.*member).clear();
			return;
		}

		(outValue.*member) = token.GetStrings(name);
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<T2> T::* memberObjects, JsonDeclaration<T2> declaration, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		(outValue.*memberObjects).clear();

		if (optional && !token.HasValue(name))
			return;

		auto& memberObjectValue = (outValue.*memberObjects);

		auto subTokens = token.GetObjectTokens(name);

		for (auto& subToken : subTokens)
		{
			memberObjectValue.emplace_back();
			for (auto& action : declaration.m_parsingActions)
			{
				action(memberObjectValue.back(), subToken);
			}
		}
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, vector<T2> T::* memberEnums, std::map<string, T2> conversionMap, bool optional)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		(outValue.*memberEnums).clear();

		if (optional && !token.HasValue(name))
			return;

		auto strs = token.GetStrings(name);

		auto& enumList = (outValue.*memberEnums);
		enumList.reserve(strs.size());

		for (auto& str : strs)
		{
			auto findIt = conversionMap.find(str);

			if (findIt == conversionMap.end())
			{
				Common::Error("%s: failed to map enum value: '%s'", name, str);
				(outValue.*memberEnums).clear();
				return;
			}

			enumList.push_back(findIt->second);
		}
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<uint32_t>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetUint32s(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<float>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetFloats(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<int32_t>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetInt32s(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<string>> T::* member)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
			(outValue.*member) = token.GetStrings(name);
		else
			(outValue.*member) = none;
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<T2>> T::* memberObject, JsonDeclaration<T2> declaration)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
		{
			(outValue.*memberObject) = vector<T2>();

			auto& memberObjectValue = *(outValue.*memberObject);

			auto subTokens = token.GetObjectTokens(name);
			memberObjectValue.reserve(subTokens.size());

			for (auto& subToken : subTokens)
			{
				memberObjectValue.emplace_back();
				for (auto& action : declaration.m_parsingActions)
				{
					action(memberObjectValue.back(), subToken);
				}
			}
		}
		else
		{
			(outValue.*member) = none;
		}
	});
}

template<typename T>
template<typename T2>
inline void Common::JsonDeclaration<T>::AddMember(const char* name, optional<vector<T2>> T::* memberEnums, std::map<string, T2> conversionMap)
{
	m_parsingActions.push_back([=](T& outValue, _Details::JsonToken& token)
	{
		if (token.HasValue(name))
		{
			auto strs = token.GetStrings(name);

			(outValue.*memberEnums) = vector<T2>();

			auto& enumList = *(outValue.*memberEnums);
			enumList.reserve(strs.size());

			for (auto& str : strs)
			{
				auto findIt = conversionMap.find(str);

				if (findIt == conversionMap.end())
				{
					Common::Error("%s: failed to map enum value: '%s'", name, str);
					(outValue.*memberEnums).clear();
					return;
				}

				enumList.push_back(findIt->second);
			}
		}
		else
		{
			(outValue.*memberEnums) = none;
		}
	});
}