#include "Precomp.h"

#include "nlohmann/json.hpp"

using namespace ::Common;
using namespace ::Common::_Details;

typedef nlohmann::basic_json<std::map, vector, string, bool, int32_t, uint32_t, float> json;

namespace Common::_Details {

class JsonEngine
{
public:
	// ctor & dtor
	JsonEngine(bool& failure, json& j);
	~JsonEngine();

public:
	// Main functionality

	bool HasValue(const char* name) const;
	bool GetBool(const char* name);
	uint32_t GetUint32(const char* name);
	float GetFloat(const char* name);
	int32_t GetInt32(const char* name);
	string GetString(const char* name);
	JsonEngine* GetObject(const char* name);

	vector<bool> GetBools(const char* name);
	vector<uint32_t> GetUint32s(const char* name);
	vector<float> GetFloats(const char* name);
	vector<int32_t> GetInt32s(const char* name);
	vector<string> GetStrings(const char* name);
	vector<JsonEngine*> GetObjects(const char* name);

private:
	// Internal functionality
	void SetFailure(const char* name);

	void SetFailure(const char* name, const char* exprectedType);

private:
	// Member variables
	bool& m_failure;
	json& m_json;
};

}

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
{
}

JsonParser::~JsonParser()
{
}

void JsonParser::ParseImpl(const string& jsonFile, std::function<void(_Details::JsonToken&)> onSuccess)
{
	std::ifstream file(jsonFile);
	auto j = json::parse(file, nullptr, false);

	if (j.is_discarded())
	{
		Common::Error("unable to parse json file '%s': contains errors", jsonFile);
		m_failure = true;
		return;
	}

	if (j.is_array())
	{
		Common::Error("unable to parse json file '%s': contains an array, use the vector overload", jsonFile);
		m_failure = true;
		return;
	}

	JsonToken token(new JsonEngine(m_failure, j));

	onSuccess(token);
}

void JsonParser::ParseImpl(const string& jsonFile, std::function<void(vector<_Details::JsonToken>&)> onSuccess)
{
	std::ifstream file(jsonFile);
	auto arr = json::parse(file, nullptr, false);

	if (arr.is_discarded())
	{
		Common::Error("unable to parse json file '%s'", jsonFile);
		m_failure = true;
		return;
	}

	if (!arr.is_array())
	{
		Common::Error("unable to parse json file '%s': does not contain an array, use the non-vector overload", jsonFile);
		m_failure = true;
		return;
	}

	vector<JsonToken> tokens;
	tokens.reserve(arr.size());

	for (auto& j : arr)
	{
		tokens.emplace_back(new JsonEngine(m_failure, j));
	}

	onSuccess(tokens);
}

JsonToken::JsonToken(JsonEngine* engine)
	: m_engine(engine)
{

}

JsonToken::JsonToken(JsonToken&& other) noexcept
	: m_engine(other.m_engine)
{
	other.m_engine = nullptr;
}

JsonToken::JsonToken(const JsonToken& other)
	: m_engine(new JsonEngine(*other.m_engine))
{
}

JsonToken::~JsonToken()
{
	delete m_engine;
}

bool JsonToken::HasValue(const char* name) const
{
	return m_engine->HasValue(name);
}

bool JsonToken::GetBool(const char* name)
{
	return m_engine->GetBool(name);
}

uint32_t JsonToken::GetUint32(const char* name)
{
	return m_engine->GetUint32(name);
}

float JsonToken::GetFloat(const char* name)
{
	return m_engine->GetFloat(name);
}

int32_t JsonToken::GetInt32(const char* name)
{
	return m_engine->GetInt32(name);
}

string JsonToken::GetString(const char* name)
{
	return m_engine->GetString(name);
}

JsonToken JsonToken::GetObjectToken(const char* name)
{
	return JsonToken(m_engine->GetObject(name));
}

vector<bool> JsonToken::GetBools(const char* name)
{
	return m_engine->GetBools(name);
}

vector<uint32_t> JsonToken::GetUint32s(const char* name)
{
	return m_engine->GetUint32s(name);
}

vector<float> JsonToken::GetFloats(const char* name)
{
	return m_engine->GetFloats(name);
}

vector<int32_t> JsonToken::GetInt32s(const char* name)
{
	return m_engine->GetInt32s(name);
}

vector<string> JsonToken::GetStrings(const char* name)
{
	return m_engine->GetStrings(name);
}

vector<JsonToken> JsonToken::GetObjectTokens(const char* name)
{
	auto objects = m_engine->GetObjects(name);

	vector<JsonToken> tokens;

	tokens.reserve(objects.size());

	for (auto& object : objects)
	{
		tokens.emplace_back(object);
	}

	return tokens;
}

JsonEngine::JsonEngine(bool& failure, json& j)
	: m_failure(failure),
	m_json(j)
{
}

JsonEngine::~JsonEngine()
{
}

bool JsonEngine::HasValue(const char* name) const
{
	return m_json.contains(name) && !m_json[name].is_null();
}

bool JsonEngine::GetBool(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return false;
	}

	if (!m_json.at(name).is_boolean())
		SetFailure(name, "boolean");

	return m_json.value<bool>(name, false);
}

uint32_t JsonEngine::GetUint32(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return 0;
	}

	if (!m_json.at(name).is_number_unsigned())
		SetFailure(name, "positive integer number");

	return m_json.value<uint32_t>(name, 0);
}

float JsonEngine::GetFloat(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return 0;
	}

	if (!m_json.at(name).is_number())
		SetFailure(name, "float number");

	return m_json.value<float>(name, 0);
}

int32_t JsonEngine::GetInt32(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return 0;
	}

	if (!m_json.at(name).is_number_integer())
		SetFailure(name, "integer number");

	return m_json.value<int32_t>(name, 0);
}

string JsonEngine::GetString(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return "";
	}

	if (!m_json.at(name).is_string())
		SetFailure(name, "string");

	return m_json.value(name, "");
}

JsonEngine* JsonEngine::GetObject(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);

		// Generate empty object to help the parser survive
		m_json[name] = {};
	}

	if (!m_json.at(name).is_object())
		SetFailure(name, "object");

	return new JsonEngine(m_failure, m_json.at(name));
}

vector<bool> JsonEngine::GetBools(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json[name].is_array())
	{
		return { GetBool(name) };
	}

	for (auto& j : m_json.at(name))
	{
		if (!j.is_boolean())
		{
			SetFailure(name, "boolean");
			break;
		}
	}

	return m_json.value<vector<bool>>(name, {});
}

vector<uint32_t> JsonEngine::GetUint32s(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json[name].is_array())
	{
		return { GetUint32(name) };
	}

	for (auto& j : m_json.at(name))
	{
		if (!j.is_number_unsigned())
		{
			SetFailure(name, "positive integer number");
			break;
		}
	}

	return m_json.value<vector<uint32_t>>(name, {});
}

vector<float> JsonEngine::GetFloats(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json.at(name).is_array())
	{
		return { GetFloat(name) };
	}

	for (auto& j : m_json.at(name))
	{
		if(!j.is_number())
		{
			SetFailure(name, "float number");
			break;
		}
	}

	return m_json.value<vector<float>>(name, {});
}

vector<int32_t> JsonEngine::GetInt32s(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json.at(name).is_array())
	{
		return { GetInt32(name) };
	}

	for (auto& j : m_json[name])
	{
		if (!j.is_number_integer())
		{
			SetFailure(name, "integer number");
			break;
		}
	}

	return m_json.value<vector<int32_t>>(name, {});
}

vector<string> JsonEngine::GetStrings(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json.at(name).is_array())
	{
		return { GetString(name) };
	}

	for (auto& j : m_json.at(name))
	{
		if (!j.is_string())
		{
			SetFailure(name, "string");
			break;
		}
	}

	return m_json.value<vector<string>>(name, {});
}

vector<JsonEngine*> JsonEngine::GetObjects(const char* name)
{
	if (!m_json.contains(name))
	{
		SetFailure(name);
		return {};
	}

	if (!m_json.at(name).is_array())
	{
		return { GetObject(name) };
	}

	for (auto& j : m_json.at(name))
	{
		if (!j.is_object())
		{
			SetFailure(name, "object");
			break;
		}
	}

	vector<JsonEngine*> tokens;
	tokens.reserve(m_json.at(name).size());

	for (auto& j : m_json[name])
	{
		tokens.push_back(new JsonEngine(m_failure, j));
	}

	return tokens;
}

void JsonEngine::SetFailure(const char* name)
{
	Common::Error("member '%s' required", name);
	m_failure = true;
}

void JsonEngine::SetFailure(const char* name, const char* exprectedType)
{
	Common::Error("member '%s' should be a %s", name, exprectedType);
	m_failure = true;
}