#pragma once

namespace Common {

class JsonParser;

namespace _Details {
class JsonEngine;
class JsonToken;
}

/**
	Declare your type for json parsing
*/
template<typename T>
class JsonDeclaration
{
	friend class JsonParser;
	template<typename T>
	friend class JsonDeclaration;
public:
	// Main functionality

	inline void AddMember(const char* name, bool T::* member, bool optional = false);
	inline void AddMember(const char* name, uint32_t T::* member, bool optional = false);
	inline void AddMember(const char* name, float T::* member, bool optional = false);
	inline void AddMember(const char* name, int32_t T::* member, bool optional = false);
	inline void AddMember(const char* name, string T::* member, bool optional = false);

	template<typename T2>
	inline void AddMember(const char* name, T2 T::* memberObject, JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void AddMember(const char* name, T2 T::* memberEnum, std::map<string, T2> conversionMap, bool optional = false);

	inline void AddMember(const char* name, optional<bool> T::* member);
	inline void AddMember(const char* name, optional<uint32_t> T::* member);
	inline void AddMember(const char* name, optional<float> T::* member);
	inline void AddMember(const char* name, optional<int32_t> T::* member);
	inline void AddMember(const char* name, optional<string> T::* member);

	template<typename T2>
	inline void AddMember(const char* name, optional<T2> T::* memberObject, JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void AddMember(const char* name, optional<T2> T::* memberEnum, std::map<string, T2> conversionMap);

	inline void AddMember(const char* name, vector<bool> T::* member, bool optional = false);
	inline void AddMember(const char* name, vector<uint32_t> T::* member, bool optional = false);
	inline void AddMember(const char* name, vector<float> T::* member, bool optional = false);
	inline void AddMember(const char* name, vector<int32_t> T::* member, bool optional = false);
	inline void AddMember(const char* name, vector<string> T::* member, bool optional = false);

	template<typename T2>
	inline void AddMember(const char* name, vector<T2> T::* memberObject, JsonDeclaration<T2> declaration, bool optional = false);

	template<typename T2>
	inline void AddMember(const char* name, vector<T2> T::* memberEnum, std::map<string, T2> conversionMap, bool optional = false);

	inline void AddMember(const char* name, optional<vector<bool>> T::* member);
	inline void AddMember(const char* name, optional<vector<uint32_t>> T::* member);
	inline void AddMember(const char* name, optional<vector<float>> T::* member);
	inline void AddMember(const char* name, optional<vector<int32_t>> T::* member);
	inline void AddMember(const char* name, optional<vector<string>> T::* member);

	template<typename T2>
	inline void AddMember(const char* name, optional<vector<T2>> T::* memberObject, JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void AddMember(const char* name, optional<vector<T2>> T::* memberEnum, std::map<string, T2> conversionMap);

private:
	// Internal declarations

	vector<std::function<void(T&, _Details::JsonToken&)>> m_parsingActions;
};

class JsonParser
{
public:
	// ctor & dtor

	JsonParser();
	~JsonParser();

public:
	// Main functionality

	template<typename T>
	optional<T> Parse(const string& jsonFile, const JsonDeclaration<T>& declaration);

	template<typename T>
	optional<vector<T>> ParseArray(const string& jsonFile, const JsonDeclaration<T>& declaration);

private:
	// Internal functionality

	void ParseImpl(const string& jsonFile, std::function<void(_Details::JsonToken&)> onSuccess);
	void ParseImpl(const string& jsonFile, std::function<void(vector<_Details::JsonToken>&)> onSuccess);

private:
	// Member variables

	bool m_failure;
};

namespace _Details {

class JsonToken
{
public:
	// ctor & dtor
	JsonToken(_Details::JsonEngine* engine);
	JsonToken(JsonToken&&) noexcept;
	JsonToken(const JsonToken&);
	~JsonToken();

public:
	// Main functionality

	bool HasValue(const char* name) const;

	bool GetBool(const char* name);
	uint32_t GetUint32(const char* name);
	float GetFloat(const char* name);
	int32_t GetInt32(const char* name);
	string GetString(const char* name);
	JsonToken GetObjectToken(const char* name);

	vector<bool> GetBools(const char* name);
	vector<uint32_t> GetUint32s(const char* name);
	vector<float> GetFloats(const char* name);
	vector<int32_t> GetInt32s(const char* name);
	vector<string> GetStrings(const char* name);
	vector<JsonToken> GetObjectTokens(const char* name);

private:
	// Member variables
	_Details::JsonEngine* m_engine;
};

} // _Details

}