#include "Precomp.h"

#include "nlohmann/json.hpp"

using namespace ::CainEngine::Common;
using namespace ::CainEngine::Common::Details;

typedef nlohmann::basic_json<std::map, std::vector, std::string, bool, int32_t, uint32_t, float>
	json;

namespace CainEngine::Common::Details {

class JsonEngine
{
public:
	// ctor & dtor
	JsonEngine(bool& failure, json& j);
	~JsonEngine();

public:
	// Main functionality

	bool hasValue(const char* name) const;
	bool getBool(const char* name);
	uint32_t getUint32(const char* name);
	float getFloat(const char* name);
	int32_t getInt32(const char* name);
	std::string getString(const char* name);
	JsonEngine* getObject(const char* name);

	std::vector<bool> getBools(const char* name);
	std::vector<uint32_t> getUint32s(const char* name);
	std::vector<float> getFloats(const char* name);
	std::vector<int32_t> getInt32s(const char* name);
	std::vector<std::string> getStrings(const char* name);
	std::vector<JsonEngine*> getObjects(const char* name);

private:
	// Internal functionality
	void setFailure(const char* name);

	void setFailure(const char* name, const char* exprectedType);

private:
	// Member variables
	bool& m_failure;
	json& m_json;
};

} // namespace CainEngine::Common::Details

struct Simple
{
	float value;
};

struct Test
{
	uint32_t value;

	Simple simple;
};

JsonParser::JsonParser()
	: m_failure(false)
{ }

JsonParser::~JsonParser()
{ }

void JsonParser::parseImpl(
	const std::string& jsonFile, std::function<void(Details::JsonToken&)> onSuccess)
{
	std::ifstream file(jsonFile);
	auto j = json::parse(file, nullptr, false);

	if(j.is_discarded())
	{
		Common::error("unable to parse json file '%s': contains errors", jsonFile);
		m_failure = true;
		return;
	}

	if(j.is_array())
	{
		Common::error(
			"unable to parse json file '%s': contains an array, use the vector overload", jsonFile);
		m_failure = true;
		return;
	}

	JsonToken token(new JsonEngine(m_failure, j));

	onSuccess(token);
}

void JsonParser::parseImpl(
	const std::string& jsonFile, std::function<void(std::vector<Details::JsonToken>&)> onSuccess)
{
	std::ifstream file(jsonFile);
	auto arr = json::parse(file, nullptr, false);

	if(arr.is_discarded())
	{
		Common::error("unable to parse json file '%s'", jsonFile);
		m_failure = true;
		return;
	}

	if(!arr.is_array())
	{
		Common::error("unable to parse json file '%s': does not contain an array, use the "
					  "non-vector overload",
			jsonFile);
		m_failure = true;
		return;
	}

	std::vector<JsonToken> tokens;
	tokens.reserve(arr.size());

	for(auto& j : arr)
	{
		tokens.emplace_back(new JsonEngine(m_failure, j));
	}

	onSuccess(tokens);
}

JsonToken::JsonToken(JsonEngine* engine)
	: m_engine(engine)
{ }

JsonToken::JsonToken(JsonToken&& other) noexcept
	: m_engine(other.m_engine)
{
	other.m_engine = nullptr;
}

JsonToken::JsonToken(const JsonToken& other)
	: m_engine(new JsonEngine(*other.m_engine))
{ }

JsonToken::~JsonToken()
{
	delete m_engine;
}

bool JsonToken::hasValue(const char* name) const
{
	return m_engine->hasValue(name);
}

bool JsonToken::getBool(const char* name)
{
	return m_engine->getBool(name);
}

uint32_t JsonToken::getUint32(const char* name)
{
	return m_engine->getUint32(name);
}

float JsonToken::getFloat(const char* name)
{
	return m_engine->getFloat(name);
}

int32_t JsonToken::getInt32(const char* name)
{
	return m_engine->getInt32(name);
}

std::string JsonToken::getString(const char* name)
{
	return m_engine->getString(name);
}

JsonToken JsonToken::getObjectToken(const char* name)
{
	return JsonToken(m_engine->getObject(name));
}

std::vector<bool> JsonToken::getBools(const char* name)
{
	return m_engine->getBools(name);
}

std::vector<uint32_t> JsonToken::getUint32s(const char* name)
{
	return m_engine->getUint32s(name);
}

std::vector<float> JsonToken::getFloats(const char* name)
{
	return m_engine->getFloats(name);
}

std::vector<int32_t> JsonToken::getInt32s(const char* name)
{
	return m_engine->getInt32s(name);
}

std::vector<std::string> JsonToken::getStrings(const char* name)
{
	return m_engine->getStrings(name);
}

std::vector<JsonToken> JsonToken::getObjectTokens(const char* name)
{
	auto objects = m_engine->getObjects(name);

	std::vector<JsonToken> tokens;

	tokens.reserve(objects.size());

	for(auto& object : objects)
	{
		tokens.emplace_back(object);
	}

	return tokens;
}

JsonEngine::JsonEngine(bool& failure, json& j)
	: m_failure(failure)
	, m_json(j)
{ }

JsonEngine::~JsonEngine()
{ }

bool JsonEngine::hasValue(const char* name) const
{
	return m_json.contains(name) && !m_json[name].is_null();
}

bool JsonEngine::getBool(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return false;
	}

	if(!m_json.at(name).is_boolean())
		setFailure(name, "boolean");

	return m_json.value<bool>(name, false);
}

uint32_t JsonEngine::getUint32(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return 0;
	}

	if(!m_json.at(name).is_number_unsigned())
		setFailure(name, "positive integer number");

	return m_json.value<uint32_t>(name, 0);
}

float JsonEngine::getFloat(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return 0;
	}

	if(!m_json.at(name).is_number())
		setFailure(name, "float number");

	return m_json.value<float>(name, 0);
}

int32_t JsonEngine::getInt32(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return 0;
	}

	if(!m_json.at(name).is_number_integer())
		setFailure(name, "integer number");

	return m_json.value<int32_t>(name, 0);
}

std::string JsonEngine::getString(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return "";
	}

	if(!m_json.at(name).is_string())
		setFailure(name, "std::string");

	return m_json.value(name, "");
}

JsonEngine* JsonEngine::getObject(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);

		// Generate empty object to help the parser survive
		m_json[name] = {};
	}

	if(!m_json.at(name).is_object())
		setFailure(name, "object");

	return new JsonEngine(m_failure, m_json.at(name));
}

std::vector<bool> JsonEngine::getBools(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json[name].is_array())
	{
		return { getBool(name) };
	}

	for(auto& j : m_json.at(name))
	{
		if(!j.is_boolean())
		{
			setFailure(name, "boolean");
			break;
		}
	}

	return m_json.value<std::vector<bool>>(name, {});
}

std::vector<uint32_t> JsonEngine::getUint32s(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json[name].is_array())
	{
		return { getUint32(name) };
	}

	for(auto& j : m_json.at(name))
	{
		if(!j.is_number_unsigned())
		{
			setFailure(name, "positive integer number");
			break;
		}
	}

	return m_json.value<std::vector<uint32_t>>(name, {});
}

std::vector<float> JsonEngine::getFloats(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json.at(name).is_array())
	{
		return { getFloat(name) };
	}

	for(auto& j : m_json.at(name))
	{
		if(!j.is_number())
		{
			setFailure(name, "float number");
			break;
		}
	}

	return m_json.value<std::vector<float>>(name, {});
}

std::vector<int32_t> JsonEngine::getInt32s(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json.at(name).is_array())
	{
		return { getInt32(name) };
	}

	for(auto& j : m_json[name])
	{
		if(!j.is_number_integer())
		{
			setFailure(name, "integer number");
			break;
		}
	}

	return m_json.value<std::vector<int32_t>>(name, {});
}

std::vector<std::string> JsonEngine::getStrings(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json.at(name).is_array())
	{
		return { getString(name) };
	}

	for(auto& j : m_json.at(name))
	{
		if(!j.is_string())
		{
			setFailure(name, "std::string");
			break;
		}
	}

	return m_json.value<std::vector<std::string>>(name, {});
}

std::vector<JsonEngine*> JsonEngine::getObjects(const char* name)
{
	if(!m_json.contains(name))
	{
		setFailure(name);
		return {};
	}

	if(!m_json.at(name).is_array())
	{
		return { getObject(name) };
	}

	for(auto& j : m_json.at(name))
	{
		if(!j.is_object())
		{
			setFailure(name, "object");
			break;
		}
	}

	std::vector<JsonEngine*> tokens;
	tokens.reserve(m_json.at(name).size());

	for(auto& j : m_json[name])
	{
		tokens.push_back(new JsonEngine(m_failure, j));
	}

	return tokens;
}

void JsonEngine::setFailure(const char* name)
{
	Common::error("member '%s' required", name);
	m_failure = true;
}

void JsonEngine::setFailure(const char* name, const char* exprectedType)
{
	Common::error("member '%s' should be a %s", name, exprectedType);
	m_failure = true;
}