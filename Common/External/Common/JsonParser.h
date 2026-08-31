#pragma once

namespace CainEngine::Common {

class JsonParser;

namespace Details {
class JsonEngine;
class JsonToken;
} // namespace Details

/**
	Declare your type for json parsing
*/
template<typename T>
class JsonDeclaration
{
	friend class JsonParser;
	template<typename T2>
	friend class JsonDeclaration;

public:
	// Main functionality

	inline void addMember(const char* name, bool T::* member, bool optional = false);
	inline void addMember(const char* name, uint32_t T::* member, bool optional = false);
	inline void addMember(const char* name, float T::* member, bool optional = false);
	inline void addMember(const char* name, int32_t T::* member, bool optional = false);
	inline void addMember(const char* name, std::string T::* member, bool optional = false);

	template<typename T2>
	inline void addMember(const char* name, T2 T::* memberObject, JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void addMember(const char* name, T2 T::* memberEnum,
		std::map<std::string, T2> conversionMap, bool optional = false);

	inline void addMember(const char* name, std::optional<bool> T::* member);
	inline void addMember(const char* name, std::optional<uint32_t> T::* member);
	inline void addMember(const char* name, std::optional<float> T::* member);
	inline void addMember(const char* name, std::optional<int32_t> T::* member);
	inline void addMember(const char* name, std::optional<std::string> T::* member);

	template<typename T2>
	inline void addMember(
		const char* name, std::optional<T2> T::* memberObject, JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void addMember(const char* name, std::optional<T2> T::* memberEnum,
		std::map<std::string, T2> conversionMap);

	inline void addMember(const char* name, std::vector<bool> T::* member, bool optional = false);
	inline void addMember(
		const char* name, std::vector<uint32_t> T::* member, bool optional = false);
	inline void addMember(const char* name, std::vector<float> T::* member, bool optional = false);
	inline void addMember(
		const char* name, std::vector<int32_t> T::* member, bool optional = false);
	inline void addMember(
		const char* name, std::vector<std::string> T::* member, bool optional = false);

	template<typename T2>
	inline void addMember(const char* name, std::vector<T2> T::* memberObject,
		JsonDeclaration<T2> declaration, bool optional = false);

	template<typename T2>
	inline void addMember(const char* name, std::vector<T2> T::* memberEnum,
		std::map<std::string, T2> conversionMap, bool optional = false);

	inline void addMember(const char* name, std::optional<std::vector<bool>> T::* member);
	inline void addMember(const char* name, std::optional<std::vector<uint32_t>> T::* member);
	inline void addMember(const char* name, std::optional<std::vector<float>> T::* member);
	inline void addMember(const char* name, std::optional<std::vector<int32_t>> T::* member);
	inline void addMember(const char* name, std::optional<std::vector<std::string>> T::* member);

	template<typename T2>
	inline void addMember(const char* name, std::optional<std::vector<T2>> T::* memberObject,
		JsonDeclaration<T2> declaration);

	template<typename T2>
	inline void addMember(const char* name, std::optional<std::vector<T2>> T::* memberEnum,
		std::map<std::string, T2> conversionMap);

private:
	// Internal declarations

	std::vector<std::function<void(T&, Details::JsonToken&)>> m_parsingActions;
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
	std::optional<T> parse(const std::string& jsonFile, const JsonDeclaration<T>& declaration);

	template<typename T>
	std::optional<std::vector<T>> parseArray(
		const std::string& jsonFile, const JsonDeclaration<T>& declaration);

private:
	// Internal functionality

	void parseImpl(const std::string& jsonFile, std::function<void(Details::JsonToken&)> onSuccess);
	void parseImpl(const std::string& jsonFile,
		std::function<void(std::vector<Details::JsonToken>&)> onSuccess);

private:
	// Member variables

	bool m_failure;
};

namespace Details {

class JsonToken
{
public:
	// ctor & dtor
	JsonToken(Details::JsonEngine* engine);
	JsonToken(JsonToken&&) noexcept;
	JsonToken(const JsonToken&);
	~JsonToken();

public:
	// Main functionality

	bool hasValue(const char* name) const;

	bool getBool(const char* name);
	uint32_t getUint32(const char* name);
	float getFloat(const char* name);
	int32_t getInt32(const char* name);
	std::string getString(const char* name);
	JsonToken getObjectToken(const char* name);

	std::vector<bool> getBools(const char* name);
	std::vector<uint32_t> getUint32s(const char* name);
	std::vector<float> getFloats(const char* name);
	std::vector<int32_t> getInt32s(const char* name);
	std::vector<std::string> getStrings(const char* name);
	std::vector<JsonToken> getObjectTokens(const char* name);

private:
	// Member variables
	Details::JsonEngine* m_engine;
};

} // namespace Details

} // namespace CainEngine::Common