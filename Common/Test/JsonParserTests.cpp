#include <Common.h>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

enum class Color
{
	Red,
	Green,
	Blue
};

struct Nested
{
	int32_t value = 0;
};

struct Widget
{
	bool enabled = false;
	bool flagWithDefault = false;
	uint32_t count = 0;
	float ratio = 0.0f;
	int32_t offset = 0;
	std::string name;
	Color color = Color::Red;
	Nested nested;
	std::optional<std::string> description;
	std::vector<int32_t> tags;
	std::vector<Nested> children;
};

std::map<std::string, Color> ColorMap()
{
	return {{"Red", Color::Red}, {"Green", Color::Green}, {"Blue", Color::Blue}};
}

JsonDeclaration<Nested> NestedDeclaration()
{
	JsonDeclaration<Nested> decl;
	decl.AddMember("value", &Nested::value);
	return decl;
}

JsonDeclaration<Widget> WidgetDeclaration()
{
	JsonDeclaration<Widget> decl;
	decl.AddMember("enabled", &Widget::enabled);
	decl.AddMember("flagWithDefault", &Widget::flagWithDefault, /* optional */ true);
	decl.AddMember("count", &Widget::count);
	decl.AddMember("ratio", &Widget::ratio);
	decl.AddMember("offset", &Widget::offset);
	decl.AddMember("name", &Widget::name);
	decl.AddMember("color", &Widget::color, ColorMap());
	decl.AddMember("nested", &Widget::nested, NestedDeclaration());
	decl.AddMember("description", &Widget::description);
	decl.AddMember("tags", &Widget::tags);
	decl.AddMember("children", &Widget::children, NestedDeclaration());
	return decl;
}

struct Item
{
	int32_t value = 0;
};

JsonDeclaration<Item> ItemDeclaration()
{
	JsonDeclaration<Item> decl;
	decl.AddMember("value", &Item::value);
	return decl;
}

class JsonParserTest : public ::testing::Test
{
protected:
	// Writes `content` to a fresh temp file and returns its path; all files
	// created this way are cleaned up in TearDown().
	std::filesystem::path WriteJson(std::string_view content)
	{
		auto path = std::filesystem::temp_directory_path() /
			("CainEngine_CommonTests_Json_" + std::to_string(m_fileCounter++) + ".json");

		std::ofstream file(path);
		file << content;
		file.close();

		m_writtenFiles.push_back(path);
		return path;
	}

	void TearDown() override
	{
		for(auto& path : m_writtenFiles)
			std::filesystem::remove(path);
	}

private:
	std::vector<std::filesystem::path> m_writtenFiles;
	int m_fileCounter = 0;
};

constexpr std::string_view CompleteWidgetJson = R"({
	"enabled": true,
	"count": 42,
	"ratio": 1.5,
	"offset": -7,
	"name": "widget-1",
	"color": "Green",
	"nested": { "value": 99 },
	"description": "a widget",
	"tags": [1, 2, 3],
	"children": [ { "value": 1 }, { "value": 2 } ]
})";

} // namespace

// -- Required scalar fields ----------------------------------------------------

TEST_F(JsonParserTest, ParsesAllFieldKindsFromCompleteJson)
{
	auto path = WriteJson(CompleteWidgetJson);

	JsonParser parser;
	auto result = parser.Parse(path.string(), WidgetDeclaration());

	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(result->enabled);
	EXPECT_EQ(42u, result->count);
	EXPECT_FLOAT_EQ(1.5f, result->ratio);
	EXPECT_EQ(-7, result->offset);
	EXPECT_EQ("widget-1", result->name);
	EXPECT_EQ(Color::Green, result->color);
	EXPECT_EQ(99, result->nested.value);
	ASSERT_TRUE(result->description.has_value());
	EXPECT_EQ("a widget", *result->description);
	EXPECT_EQ((std::vector<int32_t>{1, 2, 3}), result->tags);
	ASSERT_EQ(2u, result->children.size());
	EXPECT_EQ(1, result->children[0].value);
	EXPECT_EQ(2, result->children[1].value);
}

TEST_F(JsonParserTest, MissingRequiredFieldFailsTheParse)
{
	// No "name" field, which is a required (non-optional) member.
	auto path = WriteJson(R"({
		"enabled": true,
		"count": 1,
		"ratio": 1.0,
		"offset": 0,
		"color": "Red",
		"nested": { "value": 0 },
		"tags": [],
		"children": []
	})");

	JsonParser parser;
	auto result = parser.Parse(path.string(), WidgetDeclaration());

	EXPECT_FALSE(result.has_value());
}

// -- The `optional` bool parameter (distinct from std::optional<T> fields) ---

TEST_F(JsonParserTest, MemberDeclaredOptionalDefaultsWhenMissing)
{
	// "flagWithDefault" is declared with the optional=true AddMember overload,
	// so omitting it from the JSON must not fail the parse.
	auto path = WriteJson(CompleteWidgetJson);

	JsonParser parser;
	auto result = parser.Parse(path.string(), WidgetDeclaration());

	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->flagWithDefault);
}

// -- std::optional<T> fields ----------------------------------------------------

TEST_F(JsonParserTest, OptionalFieldIsNulloptWhenAbsent)
{
	auto path = WriteJson(R"({
		"enabled": true,
		"count": 1,
		"ratio": 1.0,
		"offset": 0,
		"name": "no-description",
		"color": "Red",
		"nested": { "value": 0 },
		"tags": [],
		"children": []
	})");

	JsonParser parser;
	auto result = parser.Parse(path.string(), WidgetDeclaration());

	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result->description.has_value());
}

// -- Enum mapping -----------------------------------------------------------------

TEST_F(JsonParserTest, InvalidEnumValueLogsAnErrorButDoesNotFailTheParse)
{
	// Unlike a missing required scalar field, a value that fails to match the
	// conversion map only logs Common::Error() and resets the member to its
	// zero value (Color::Red) - it does not set the parser's failure flag, so
	// Parse() still succeeds. This is existing (if perhaps surprising)
	// behavior, captured here so a future change to it is a deliberate one.
	auto path = WriteJson(R"({
		"enabled": true,
		"count": 1,
		"ratio": 1.0,
		"offset": 0,
		"name": "widget",
		"color": "Purple",
		"nested": { "value": 0 },
		"tags": [],
		"children": []
	})");

	JsonParser parser;
	auto result = parser.Parse(path.string(), WidgetDeclaration());

	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(Color::Red, result->color);
}

// -- Parse() vs. ParseArray() shape mismatches -------------------------------

TEST_F(JsonParserTest, ParseOnATopLevelArrayFails)
{
	auto path = WriteJson(R"([ { "value": 1 } ])");

	JsonParser parser;
	auto result = parser.Parse(path.string(), ItemDeclaration());

	EXPECT_FALSE(result.has_value());
}

TEST_F(JsonParserTest, ParseArrayOnATopLevelObjectFails)
{
	auto path = WriteJson(R"({ "value": 1 })");

	JsonParser parser;
	auto result = parser.ParseArray(path.string(), ItemDeclaration());

	EXPECT_FALSE(result.has_value());
}

TEST_F(JsonParserTest, ParseArrayRoundTripsEachElement)
{
	auto path = WriteJson(R"([ { "value": 1 }, { "value": 2 }, { "value": 3 } ])");

	JsonParser parser;
	auto result = parser.ParseArray(path.string(), ItemDeclaration());

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(3u, result->size());
	EXPECT_EQ(1, (*result)[0].value);
	EXPECT_EQ(2, (*result)[1].value);
	EXPECT_EQ(3, (*result)[2].value);
}

// -- Malformed input ----------------------------------------------------------------

TEST_F(JsonParserTest, MalformedJsonFails)
{
	auto path = WriteJson("{ this is not valid json ");

	JsonParser parser;
	auto result = parser.Parse(path.string(), ItemDeclaration());

	EXPECT_FALSE(result.has_value());
}

TEST_F(JsonParserTest, NonexistentFileFailsGracefully)
{
	JsonParser parser;
	auto result = parser.Parse(
		(std::filesystem::temp_directory_path() / "CainEngine_CommonTests_Json_DoesNotExist.json")
			.string(),
		ItemDeclaration());

	EXPECT_FALSE(result.has_value());
}
