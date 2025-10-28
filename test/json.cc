#include <gtest/gtest.h>
#include <string_view>
#include "json.h"

TEST(JSONTest, constructor_primitive) {
    infinite::JSON i { 1 };
    ASSERT_EQ(i.get_kind(), infinite::JSONKind::INTEGER);

    infinite::JSON f { 1.1 };
    ASSERT_EQ(f.get_kind(), infinite::JSONKind::FLOAT);

    infinite::JSON b { true };
    ASSERT_EQ(b.get_kind(), infinite::JSONKind::BOOL);
}

TEST(JSONTest, constructor_string) {
    infinite::JSON s1 { "apple" };
    ASSERT_EQ(s1.get_kind(), infinite::JSONKind::STRING);

    char str[] = {'p', 'e', 'a', 'r', '\0'};
    infinite::JSON s2 { str };
    ASSERT_EQ(s2.get_kind(), infinite::JSONKind::STRING);

    infinite::JSON s3 { std::string_view("strawberry") };
    ASSERT_EQ(s3.get_kind(), infinite::JSONKind::STRING);

    infinite::JSON s4 { std::string("orange") };
    ASSERT_EQ(s4.get_kind(), infinite::JSONKind::STRING);
}

TEST(JSONTest, constructor_object) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.get_kind(), infinite::JSONKind::OBJECT);
    ASSERT_EQ(o["a"].get_kind(), infinite::JSONKind::INTEGER);
    ASSERT_EQ(o["b"].get_kind(), infinite::JSONKind::STRING);
}

TEST(JSONTest, constructor_array) {
    infinite::JSON a1 = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a1.get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_EQ(a1[0].get_kind(), infinite::JSONKind::INTEGER);
    ASSERT_EQ(a1[1].get_kind(), infinite::JSONKind::FLOAT);
    ASSERT_EQ(a1[2].get_kind(), infinite::JSONKind::STRING);

    infinite::JSON a2 = infinite::JSON::array({1, 0.1, "banana"});
    ASSERT_EQ(a2.get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_EQ(a2[0].get_kind(), infinite::JSONKind::INTEGER);
    ASSERT_EQ(a2[1].get_kind(), infinite::JSONKind::FLOAT);
    ASSERT_EQ(a2[2].get_kind(), infinite::JSONKind::STRING);
}

TEST(JSONTest, constructor_null) {
    infinite::JSON n1 = infinite::JSON::null();
    ASSERT_EQ(n1.get_kind(), infinite::JSONKind::EMPTY);

    infinite::JSON n2 = infinite::JSON::null();
    ASSERT_EQ(n2.get_kind(), infinite::JSONKind::EMPTY);
}

TEST(JSONTest, constructor_complex1) {
    infinite::JSON o {
        {"model", "qwen/qwen3-32b"},
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
    ASSERT_EQ(o.get_kind(), infinite::JSONKind::OBJECT);
    ASSERT_EQ(o["model"].get_kind(), infinite::JSONKind::STRING);
    ASSERT_EQ(o["messages"].get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_EQ(o["tools"][0]["function"]["parameters"]["properties"]["city"]["type"].get_kind(), infinite::JSONKind::STRING);
}

TEST(JSONTest, constructor_complex2) {
    infinite::JSON a = infinite::JSON::array({
        infinite::JSON::array(1, infinite::json::null, 3),
        infinite::JSON::array({2, true, {{"a", "apple"}}})
    });
    ASSERT_EQ(a.get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_EQ(a[0].get_kind(), infinite::JSONKind::ARRAY);
    ASSERT_EQ(a[0][1].get_kind(), infinite::JSONKind::EMPTY);
    ASSERT_EQ(a[0][2].get_kind(), infinite::JSONKind::INTEGER);
    ASSERT_EQ(a[1][2].get_kind(), infinite::JSONKind::OBJECT);
    ASSERT_EQ(a[1][2]["a"].get_kind(), infinite::JSONKind::STRING);
}

TEST(JSONTest, get_kind) {
    infinite::JSON i { 1 };
    ASSERT_EQ(i.get_kind(), infinite::JSONKind::INTEGER);

    infinite::JSON f { 1.1 };
    ASSERT_EQ(f.get_kind(), infinite::JSONKind::FLOAT);

    infinite::JSON b { true };
    ASSERT_EQ(b.get_kind(), infinite::JSONKind::BOOL);

    infinite::JSON s { "abc" };
    ASSERT_EQ(s.get_kind(), infinite::JSONKind::STRING);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.get_kind(), infinite::JSONKind::OBJECT);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a.get_kind(), infinite::JSONKind::ARRAY);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_EQ(n.get_kind(), infinite::JSONKind::EMPTY);
}

TEST(JSONTest, is_object) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_object());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_object());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_object());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_object());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_TRUE(o.is_object());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_object());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_object());
}

TEST(JSONTest, is_array) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_array());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_array());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_array());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_array());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_array());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_TRUE(a.is_array());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_array());
}

TEST(JSONTest, is_string) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_string());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_string());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_string());

    infinite::JSON s { "abc" };
    ASSERT_TRUE(s.is_string());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_string());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_string());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_string());
}

TEST(JSONTest, is_integer) {
    infinite::JSON i { 1 };
    ASSERT_TRUE(i.is_integer());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_integer());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_integer());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_integer());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_integer());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_integer());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_integer());
}

TEST(JSONTest, is_float) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_float());

    infinite::JSON f { 1.1 };
    ASSERT_TRUE(f.is_float());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_float());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_float());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_float());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_float());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_float());
}

TEST(JSONTest, is_bool) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_bool());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_bool());

    infinite::JSON b { true };
    ASSERT_TRUE(b.is_bool());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_bool());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_bool());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_bool());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.is_bool());
}

TEST(JSONTest, is_null) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i.is_null());

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.is_null());

    infinite::JSON b { true };
    ASSERT_FALSE(b.is_null());

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.is_null());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.is_null());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.is_null());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_TRUE(n.is_null());
}

TEST(JSONTest, as_int) {
    infinite::JSON i { 1 };
    ASSERT_EQ(i.as<int>(), 1);

    infinite::JSON f { 1.1 };
    ASSERT_THROW(f.as<int>(), infinite::JSONValueException);

    infinite::JSON b { true };
    ASSERT_THROW(b.as<int>(), infinite::JSONValueException);

    infinite::JSON s { "abc" };
    ASSERT_THROW(s.as<int>(), infinite::JSONValueException);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_THROW(o.as<int>(), infinite::JSONValueException);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_THROW(a.as<int>(), infinite::JSONValueException);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_THROW(n.as<int>(), infinite::JSONValueException);
}

TEST(JSONTest, as_double) {
    infinite::JSON i { 1 };
    ASSERT_THROW(i.as<double>(), infinite::JSONValueException);

    infinite::JSON f { 1.1 };
    ASSERT_EQ(f.as<double>(), 1.1);

    infinite::JSON b { true };
    ASSERT_THROW(b.as<double>(), infinite::JSONValueException);

    infinite::JSON s { "abc" };
    ASSERT_THROW(s.as<double>(), infinite::JSONValueException);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_THROW(o.as<double>(), infinite::JSONValueException);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_THROW(a.as<double>(), infinite::JSONValueException);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_THROW(n.as<double>(), infinite::JSONValueException);
}

TEST(JSONTest, as_bool) {
    infinite::JSON i { 1 };
    ASSERT_THROW(i.as<bool>(), infinite::JSONValueException);

    infinite::JSON f { 1.1 };
    ASSERT_THROW(f.as<bool>(), infinite::JSONValueException);

    infinite::JSON b { true };
    ASSERT_EQ(b.as<bool>(), true);

    infinite::JSON s { "abc" };
    ASSERT_THROW(s.as<bool>(), infinite::JSONValueException);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_THROW(o.as<bool>(), infinite::JSONValueException);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_THROW(a.as<bool>(), infinite::JSONValueException);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_THROW(n.as<bool>(), infinite::JSONValueException);
}

TEST(JSONTest, as_string) {
    infinite::JSON i { 1 };
    ASSERT_THROW(i.as<std::string_view>(), infinite::JSONValueException);

    infinite::JSON f { 1.1 };
    ASSERT_THROW(f.as<std::string_view>(), infinite::JSONValueException);

    infinite::JSON b { true };
    ASSERT_THROW(b.as<std::string_view>(), infinite::JSONValueException);

    infinite::JSON s { "abc" };
    ASSERT_EQ(s.as<std::string_view>(), "abc");

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_THROW(o.as<std::string_view>(), infinite::JSONValueException);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_THROW(a.as<std::string_view>(), infinite::JSONValueException);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_THROW(n.as<std::string_view>(), infinite::JSONValueException);
}

TEST(JSONTest, boolean) {
    infinite::JSON b_true { true };
    ASSERT_TRUE(b_true);

    infinite::JSON b_false { false };
    ASSERT_FALSE(b_false);
    ASSERT_TRUE(!b_false);

    infinite::JSON i { 1 };
    ASSERT_FALSE(i);

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f);

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n);
}

TEST(JSONTest, index_string) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_TRUE(o["a"] == 1);
    ASSERT_TRUE(o["b"] == "apple");
    ASSERT_TRUE(o["c"].is_null());

    infinite::JSON i { 1 };
    ASSERT_TRUE(i["a"].is_null());

    infinite::JSON f { 1.1 };
    ASSERT_TRUE(f["a"].is_null());

    infinite::JSON b { true };
    ASSERT_TRUE(b["a"].is_null());

    infinite::JSON s { "abc" };
    ASSERT_TRUE(s["a"].is_null());

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_TRUE(a["a"].is_null());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_TRUE(n["a"].is_null());
}

TEST(JSONTest, index_int) {
    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_TRUE(a[0] == 1);
    ASSERT_TRUE(a[1] == 0.1);
    ASSERT_TRUE(a[2] == "banana");
    ASSERT_TRUE(a[4].is_null());
    
    infinite::JSON i { 1 };
    ASSERT_TRUE(i[0].is_null());

    infinite::JSON f { 1.1 };
    ASSERT_TRUE(f[0].is_null());

    infinite::JSON b { true };
    ASSERT_TRUE(b[0].is_null());

    infinite::JSON s { "abc" };
    ASSERT_TRUE(s[0].is_null());

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_TRUE(o[0].is_null());

    infinite::JSON n = infinite::JSON::null();
    ASSERT_TRUE(n[0].is_null());
}

TEST(JSONTest, eq_int) {
    infinite::JSON i { 1 };
    ASSERT_TRUE(i == 1);
    ASSERT_FALSE(i == 2);

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f == 1);

    infinite::JSON b { true };
    ASSERT_FALSE(b == 1);

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s == 1);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o == 1);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a == 1);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n == 1);
}

TEST(JSONTest, eq_double) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i == 1.0);

    infinite::JSON f { 1.1 };
    ASSERT_TRUE(f == 1.1);
    ASSERT_FALSE(f == 10.0);

    infinite::JSON b { true };
    ASSERT_FALSE(b == 1.1);

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s == 1.1);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o == 1.1);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a == 1.1);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n == 1.1);
}

TEST(JSONTest, eq_bool) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i == true);

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f == true);

    infinite::JSON b { true };
    ASSERT_TRUE(b == true);
    ASSERT_FALSE(b == false);

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s == true);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o == true);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a == true);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n == true);
}

TEST(JSONTest, eq_string) {
    infinite::JSON i { 1 };
    ASSERT_FALSE(i == "abc");

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f == "abc");

    infinite::JSON b { true };
    ASSERT_FALSE(b == "abc");

    infinite::JSON s { "abc" };
    ASSERT_TRUE(s == "abc");
    ASSERT_FALSE(s == "banana");

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o == "abc");

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a == "abc");

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n == "abc");
}

TEST(JSONTest, size) {
    infinite::JSON i { 1 };
    ASSERT_EQ(i.size(), 1);

    infinite::JSON f { 1.1 };
    ASSERT_EQ(f.size(), 1);

    infinite::JSON b { true };
    ASSERT_EQ(b.size(), 1);

    infinite::JSON s { "abc" };
    ASSERT_EQ(s.size(), 1);

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a.size(), 3);

    infinite::JSON n = infinite::JSON::null();
    ASSERT_EQ(n.size(), 0);
}

TEST(JSONTest, add_pair) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    ASSERT_TRUE(o.add("c", "cherry"));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    ASSERT_TRUE(o.add("c", "coconut"));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add("a", "apple"));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add("a", "apple"));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add("a", "apple"));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add("a", "apple"));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add("a", "apple"));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add("a", "apple"));
}

TEST(JSONTest, add_pair_ref) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    infinite::JSON j1 { "cherry" };
    ASSERT_TRUE(o.add("c", j1));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    infinite::JSON j2 { "coconut" };
    ASSERT_TRUE(o.add("c", j2));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add("a", j1));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add("a", j1));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add("a", j1));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add("a", j1));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add("a", j1));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add("a", j1));
}

TEST(JSONTest, add_pair_batch) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    ASSERT_TRUE(o.add({{"c", "cherry"}}));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    ASSERT_TRUE(o.add({{"c", "coconut"}}));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add({{"a", "apple"}}));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add({{"a", "apple"}}));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add({{"a", "apple"}}));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add({{"a", "apple"}}));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add({{"a", "apple"}}));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add({{"a", "apple"}}));
}

TEST(JSONTest, add_or_replace) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    ASSERT_TRUE(o.add_or_replace("c", "cherry"));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    ASSERT_TRUE(o.add_or_replace("c", "coconut"));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "coconut");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add_or_replace("a", "apple"));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add_or_replace("a", "apple"));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add_or_replace("a", "apple"));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add_or_replace("a", "apple"));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add_or_replace("a", "apple"));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add_or_replace("a", "apple"));
}

TEST(JSONTest, add_or_replace_ref) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    infinite::JSON j1 { "cherry" };
    ASSERT_TRUE(o.add_or_replace("c", j1));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    infinite::JSON j2 { "coconut" };
    ASSERT_TRUE(o.add_or_replace("c", j2));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "coconut");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add_or_replace("a", j1));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add_or_replace("a", j1));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add_or_replace("a", j1));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add_or_replace("a", j1));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add_or_replace("a", j1));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add_or_replace("a", j1));
}

TEST(JSONTest, add_or_replace_batch) {
    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_EQ(o.size(), 2);
    ASSERT_TRUE(o["c"].is_null());
    
    ASSERT_TRUE(o.add_or_replace({{"c", "cherry"}}));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "cherry");

    ASSERT_TRUE(o.add_or_replace({{"c", "coconut"}}));
    ASSERT_EQ(o.size(), 3);
    ASSERT_EQ(o["c"], "coconut");

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add_or_replace({{"a", "apple"}}));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add_or_replace({{"a", "apple"}}));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add_or_replace({{"a", "apple"}}));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add_or_replace({{"a", "apple"}}));

    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_FALSE(a.add_or_replace({{"a", "apple"}}));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add_or_replace({{"a", "apple"}}));
}

TEST(JSONTest, add_array) {
    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a.size(), 3);
    ASSERT_TRUE(a[3].is_null());
    
    ASSERT_TRUE(a.add(10));
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a[3], 10);

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add(10));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add(10));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add(10));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add(10));

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.add(10));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add(10));
}

TEST(JSONTest, add_array_ref) {
    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a.size(), 3);
    ASSERT_TRUE(a[3].is_null());
    
    infinite::JSON j { 10 };
    ASSERT_TRUE(a.add(j));
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a[3], 10);

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add(j));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add(j));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add(j));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add(j));

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.add(j));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add(j));
}

TEST(JSONTest, add_array_batch) {
    infinite::JSON a = infinite::JSON::array(1, 0.1, "banana");
    ASSERT_EQ(a.size(), 3);
    ASSERT_TRUE(a[4].is_null());
    
    ASSERT_TRUE(a.add({1, 2}));
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a[4], 2);

    infinite::JSON i { 1 };
    ASSERT_FALSE(i.add({1, 2}));

    infinite::JSON f { 1.1 };
    ASSERT_FALSE(f.add({1, 2}));

    infinite::JSON b { true };
    ASSERT_FALSE(b.add({1, 2}));

    infinite::JSON s { "abc" };
    ASSERT_FALSE(s.add({1, 2}));

    infinite::JSON o { {"a", 1}, {"b", "apple"} };
    ASSERT_FALSE(o.add({1, 2}));

    infinite::JSON n = infinite::JSON::null();
    ASSERT_FALSE(n.add({1, 2}));
}

TEST(JSONTest, c_object_add_int) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    ASSERT_EQ(infinite_json_object_add_int(o, "a", 1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == 1);

    ASSERT_EQ(infinite_json_object_add_int(o, "a", 2), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == 2);

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_int(a, "a", 1), 0);
    infinite_free_json(a);
}

TEST(JSONTest, c_object_add_double) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    ASSERT_EQ(infinite_json_object_add_double(o, "a", 1.1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == 1.1);

    ASSERT_EQ(infinite_json_object_add_double(o, "a", 2.1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == 2.1);

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_double(a, "a", 1.0), 0);
    infinite_free_json(a);
}

TEST(JSONTest, c_object_add_bool) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    ASSERT_EQ(infinite_json_object_add_bool(o, "a", 1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"]);

    ASSERT_EQ(infinite_json_object_add_bool(o, "a", 0), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_FALSE(j["a"]);

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_bool(a, "a", 1), 0);
    infinite_free_json(a);
}

TEST(JSONTest, c_object_add_string) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    ASSERT_EQ(infinite_json_object_add_string(o, "a", "apple"), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == "apple");

    ASSERT_EQ(infinite_json_object_add_string(o, "a", "banana"), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"] == "banana");

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_string(a, "a", "cherry"), 0);
    infinite_free_json(a);
}

TEST(JSONTest, c_object_add_object) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    InfiniteJSONObjectHandle new1 = infinite_make_json_object();
    infinite_json_object_add_int(new1, "b", 1);
    ASSERT_EQ(infinite_json_object_add_object(o, "a", new1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"]["b"] == 1);
    

    InfiniteJSONObjectHandle new2 = infinite_make_json_object();
    infinite_json_object_add_int(new2, "b", 2);
    ASSERT_EQ(infinite_json_object_add_object(o, "a", new2), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"]["b"] == 2);

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_object(a, "a", new1), 0);
    infinite_free_json(a);
    infinite_free_json(new1);
    infinite_free_json(new2);
}

TEST(JSONTest, c_object_add_array) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    InfiniteJSONObjectHandle new1 = infinite_make_json_array();
    infinite_json_array_add_int(new1, 1);
    ASSERT_EQ(infinite_json_object_add_array(o, "a", new1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"][0] == 1);
    

    InfiniteJSONObjectHandle new2 = infinite_make_json_array();
    infinite_json_array_add_int(new2, 2);
    ASSERT_EQ(infinite_json_object_add_array(o, "a", new2), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"][0] == 2);

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_array(a, "a", new1), 0);
    infinite_free_json(a);
    infinite_free_json(new1);
    infinite_free_json(new2);
}

TEST(JSONTest, c_object_add_null) {
    InfiniteJSONObjectHandle o = infinite_make_json_object();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(o);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j["a"].is_null());

    ASSERT_EQ(infinite_json_object_add_null(o, "a"), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j["a"].is_null());

    infinite_free_json(o);

    InfiniteJSONArrayHandle a = infinite_make_json_array();
    ASSERT_EQ(infinite_json_object_add_null(a, "a"), 0);
    infinite_free_json(a);
}

TEST(JSONTest, c_array_add_int) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    ASSERT_EQ(infinite_json_array_add_int(a, 1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0] == 1);

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_int(o, 1), 0);
    infinite_free_json(o);
}

TEST(JSONTest, c_array_add_double) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    ASSERT_EQ(infinite_json_array_add_double(a, 1.1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0] == 1.1);

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_double(o, 1.1), 0);
    infinite_free_json(o);
}

TEST(JSONTest, c_array_add_bool) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    ASSERT_EQ(infinite_json_array_add_bool(a, 1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0]);

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_bool(o, 1), 0);
    infinite_free_json(o);
}

TEST(JSONTest, c_array_add_string) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    ASSERT_EQ(infinite_json_array_add_string(a, "apple"), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0] == "apple");

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_string(o, "apple"), 0);
    infinite_free_json(o);
}

TEST(JSONTest, c_array_add_object) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    InfiniteJSONObjectHandle new1 = infinite_make_json_object();
    infinite_json_object_add_int(new1, "a", 1);
    ASSERT_EQ(infinite_json_array_add_object(a, new1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0]["a"] == 1);

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_object(a, new1), 0);
    infinite_free_json(o);
    infinite_free_json(new1);
}

TEST(JSONTest, c_array_add_array) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    InfiniteJSONArrayHandle new1 = infinite_make_json_array();
    infinite_json_array_add_int(new1, 1);
    ASSERT_EQ(infinite_json_array_add_array(a, new1), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0][0] == 1);

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_array(a, new1), 0);
    infinite_free_json(o);
    infinite_free_json(new1);
}

TEST(JSONTest, c_array_add_null) {
    InfiniteJSONArrayHandle a = infinite_make_json_array();
    infinite::JSON& j = *reinterpret_cast<infinite::JSON*>(a);
    ASSERT_EQ(j.size(), 0);
    ASSERT_TRUE(j[0].is_null());

    ASSERT_EQ(infinite_json_array_add_null(a), 1);
    ASSERT_EQ(j.size(), 1);
    ASSERT_TRUE(j[0].is_null());

    infinite_free_json(a);

    InfiniteJSONObjectHandle o = infinite_make_json_object();
    ASSERT_EQ(infinite_json_array_add_null(o), 0);
    infinite_free_json(o);
}
