#include <gtest/gtest.h>
#include <string_view>
#include "json.h"

TEST(JSONTest, INTEGER) {
    infinite::JSON j { 10 };

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::INTEGER);
    ASSERT_TRUE(j.is_integer());
    ASSERT_FALSE(j.is_string());
    ASSERT_FALSE(j.is_float());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_null());
    ASSERT_FALSE(j.is_object());
    ASSERT_FALSE(j.is_array());

    ASSERT_TRUE(j == 10);
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == "abc");
    ASSERT_FALSE(j == 1.0);
    ASSERT_FALSE(j == true);

    ASSERT_EQ(j.as<int>(), 10);
    ASSERT_THROW(j.as<std::string_view>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j['a'], infinite::json::null);
    ASSERT_EQ(j[0], infinite::json::null);
}

TEST(JSONTest, FLOAT) {
    infinite::JSON j { 1.1 };

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::FLOAT);
    ASSERT_TRUE(j.is_float());
    ASSERT_FALSE(j.is_string());
    ASSERT_FALSE(j.is_integer());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_null());
    ASSERT_FALSE(j.is_object());
    ASSERT_FALSE(j.is_array());

    ASSERT_TRUE(j == 1.1);
    ASSERT_FALSE(j == 10.0);
    ASSERT_FALSE(j == "abc");
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == true);

    ASSERT_EQ(j.as<double>(), 1.1);
    ASSERT_THROW(j.as<std::string_view>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j['a'], infinite::json::null);
    ASSERT_EQ(j[0], infinite::json::null);
}

TEST(JSONTest, STRING) {
    infinite::JSON j { "abc" };

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::STRING);
    ASSERT_TRUE(j.is_string());
    ASSERT_FALSE(j.is_integer());
    ASSERT_FALSE(j.is_float());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_null());
    ASSERT_FALSE(j.is_object());
    ASSERT_FALSE(j.is_array());

    ASSERT_TRUE(j == "abc");
    ASSERT_FALSE(j == "abcd");
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == 1.0);
    ASSERT_FALSE(j == true);

    ASSERT_EQ(j.as<std::string_view>(), "abc");
    ASSERT_THROW(j.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j['a'], infinite::json::null);
    ASSERT_EQ(j[0], infinite::json::null);
}

TEST(JSONTest, BOOL) {
    infinite::JSON t { true };

    ASSERT_EQ(t.get_kind(), infinite::JSONKind::BOOL);
    ASSERT_TRUE(t.is_bool());
    ASSERT_FALSE(t.is_integer());
    ASSERT_FALSE(t.is_float());
    ASSERT_FALSE(t.is_string());
    ASSERT_FALSE(t.is_null());
    ASSERT_FALSE(t.is_object());
    ASSERT_FALSE(t.is_array());

    ASSERT_TRUE(t);
    ASSERT_TRUE(t == true);
    ASSERT_FALSE(t == "abc");
    ASSERT_FALSE(t == "abcd");
    ASSERT_FALSE(t == 1);
    ASSERT_FALSE(t == 1.0);
    
    ASSERT_EQ(t.as<bool>(), true);
    ASSERT_THROW(t.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(t.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(t.as<std::string_view>(), infinite::JSONValueException);

    ASSERT_EQ(t['a'], infinite::json::null);
    ASSERT_EQ(t[0], infinite::json::null);

    infinite::JSON f { false };
    ASSERT_FALSE(f);
}

TEST(JSONTest, EMPTY) {
    infinite::JSON j = infinite::JSON::null();

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::EMPTY);
    ASSERT_TRUE(j.is_null());
    ASSERT_FALSE(j.is_integer());
    ASSERT_FALSE(j.is_float());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_string());
    ASSERT_FALSE(j.is_object());
    ASSERT_FALSE(j.is_array());

    ASSERT_FALSE(j == "abc");
    ASSERT_FALSE(j == "abcd");
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == 1.0);
    ASSERT_FALSE(j == true);

    ASSERT_THROW(j.as<std::string_view>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j['a'], infinite::json::null);
    ASSERT_EQ(j[0], infinite::json::null);
}

TEST(JSONTest, EMPTY_predef) {
    infinite::JSON j = infinite::json::null;
    ASSERT_TRUE(j.is_null());
}

TEST(JSONTest, OBJECT) {
    infinite::JSON j {
        {"a", 1},
        {"b", 1.1},
        {"c", true},
        {"d", "apple"}
    };

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::OBJECT);
    ASSERT_TRUE(j.is_object());
    ASSERT_FALSE(j.is_integer());
    ASSERT_FALSE(j.is_float());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_string());
    ASSERT_FALSE(j.is_null());
    ASSERT_FALSE(j.is_array());

    ASSERT_FALSE(j == "abc");
    ASSERT_FALSE(j == "abcd");
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == 1.0);
    ASSERT_FALSE(j == true);

    ASSERT_THROW(j.as<std::string_view>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j[0], infinite::json::null);
    ASSERT_TRUE(j["a"] == 1);
    ASSERT_TRUE(j["b"] == 1.1);
    ASSERT_TRUE(j["c"]);
    ASSERT_TRUE(j["d"] == "apple");
    ASSERT_TRUE(j["e"].is_null());
}

TEST(JSONTest, ARRAY) {
    infinite::JSON j = infinite::JSON::array(1, 1.1, true, "apple");

    ASSERT_EQ(j.get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_TRUE(j.is_array());
    ASSERT_FALSE(j.is_integer());
    ASSERT_FALSE(j.is_float());
    ASSERT_FALSE(j.is_bool());
    ASSERT_FALSE(j.is_string());
    ASSERT_FALSE(j.is_null());
    ASSERT_FALSE(j.is_object());

    ASSERT_FALSE(j == "abc");
    ASSERT_FALSE(j == "abcd");
    ASSERT_FALSE(j == 1);
    ASSERT_FALSE(j == 1.0);
    ASSERT_FALSE(j == true);

    ASSERT_THROW(j.as<std::string_view>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<int>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<double>(), infinite::JSONValueException);
    ASSERT_THROW(j.as<bool>(), infinite::JSONValueException);

    ASSERT_EQ(j["a"], infinite::json::null);
    ASSERT_TRUE(j[0] == 1);
    ASSERT_TRUE(j[1] == 1.1);
    ASSERT_TRUE(j[2]);
    ASSERT_TRUE(j[3] == "apple");
    ASSERT_TRUE(j[4].is_null());
}

TEST(JSONTest, COMPLEX1) {
    infinite::JSON j {
        {"model", "qwen/qwen3-32b"},
        {"reasoning_effort", "medium"},
        {"messages", infinite::JSON::array({
            {
                {"role", "system"},
                {"content", "You're a helpful assistant."}
            },
            {
                {"role", "user"},
                {"content", "What's the weather in Roma today?"}
            }
        })},
        {"tool_choice", "auto"},
        {"tools", infinite::JSON::array({
            {
                {"type", "function"}, 
                {"function", {
                    {"name", "get_weather"}, 
                    {"description", "Get weather of the specific city in the specific day."},
                    {"parameters", {
                        {"type", "object"},
                        {"properties", {
                            {"city", {{"type", "string"}, {"description", "The city name"}}},
                            {"date", {{"type", "string"}, {"description", "The date, which in the format \"2019-03-18\""}}}
                        }}
                    }}
                }}
            }
        })},
        {"stream", true}
    };

    ASSERT_EQ(j["messages"][0]["role"], "system");
    ASSERT_EQ(j["tools"][0]["function"]["parameters"]["properties"]["city"]["description"], "The city name");
}

TEST(JSONTest, COMPLEX2) {
    infinite::JSON j = infinite::JSON::array({
        infinite::JSON::array(1, 2, 3),
        infinite::JSON::array({"a", "b", "c"})
    });

    ASSERT_EQ(j[0][1], 2);
    ASSERT_EQ(j[1][2], "c");
}