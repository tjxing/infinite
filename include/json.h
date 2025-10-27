#ifndef _JSON_H_
#define _JSON_H_

#ifdef __cplusplus
extern "C" {
#endif



////////////////////////////////////////

#ifdef __cplusplus
}

#include <concepts>
#include <cstddef>
#include <string_view>
#include <variant>
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <exception>
#include <string>

namespace infinite {

    enum class JSONKind
    {
        INTEGER,
        FLOAT,
        BOOL,
        STRING,
        OBJECT,
        ARRAY,
        EMPTY
    };

    template<typename T>
    concept JSONPrimitiveType = 
        std::same_as<T, int>            ||
        std::same_as<T, double>         ||
        std::same_as<T, bool>           ||
        std::same_as<T, std::string_view>;

    template<typename T>
    concept JSONInitPrimitiveType = JSONPrimitiveType<T> || std::convertible_to<T, std::string_view>;

    template<JSONInitPrimitiveType T> inline constexpr JSONKind JSONPrimitiveKind = JSONKind::EMPTY;
    template<> inline constexpr JSONKind JSONPrimitiveKind<int> = JSONKind::INTEGER;
    template<> inline constexpr JSONKind JSONPrimitiveKind<double> = JSONKind::FLOAT;
    template<> inline constexpr JSONKind JSONPrimitiveKind<bool> = JSONKind::BOOL;

    template<JSONInitPrimitiveType T>
    requires (std::convertible_to<T, std::string_view>)
    inline constexpr JSONKind JSONPrimitiveKind<T> = JSONKind::STRING;

    class JSONValueException : public std::exception
    {
    private:
        std::string message;

    public:
        explicit JSONValueException(const char* name)
            : message(std::string("JSON is not a") + name) {}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };

    class JSON
    {
    private:
        using value_type = std::variant<
            int, double, bool,
            std::string_view,
            std::unordered_map<std::string_view, JSON>,
            std::vector<JSON>>;

        JSONKind kind;
        value_type value;

    public:
        template<JSONInitPrimitiveType T, JSONKind K = JSONPrimitiveKind<T>>
        JSON(T v): kind(K), value(v) {}
        JSON(std::initializer_list<std::pair<const std::string_view, JSON>> data): kind(JSONKind::OBJECT), value(data) {}

        static JSON array(std::initializer_list<JSON> data) {
            JSON result = JSON(0);
            result.kind = JSONKind::ARRAY;
            result.value = std::vector<JSON>(data);
            return result;
        }

        template<typename... Args>
        requires (std::convertible_to<Args, JSON> && ...)
        static JSON array(Args... data) {
            return array({data...});
        }

        static JSON null() {
            JSON result = JSON(0);
            result.kind = JSONKind::EMPTY;
            return result;
        }

        inline JSONKind get_kind() const { return kind; }
        inline bool is_object() const { return kind == JSONKind::OBJECT; }
        inline bool is_array() const { return kind == JSONKind::ARRAY; }
        inline bool is_string() const { return kind == JSONKind::STRING; }
        inline bool is_integer() const { return kind == JSONKind::INTEGER; }
        inline bool is_float() const { return kind == JSONKind::FLOAT; }
        inline bool is_bool() const { return kind == JSONKind::BOOL; }
        inline bool is_null() const { return kind == JSONKind::EMPTY; }

        template<JSONPrimitiveType T, JSONKind K = JSONPrimitiveKind<T>>
        inline const T& as() const {
            const T *v = nullptr;
            if (is_null() || (v = std::get_if<static_cast<int>(K)>(&value)) == nullptr) {
                throw JSONValueException(typeid(T).name());
            }
            return *v;
        }

        template<JSONInitPrimitiveType T, JSONKind K = JSONPrimitiveKind<T>>
        inline bool operator==(T v) const { return kind == K && *std::get_if<static_cast<int>(K)>(&value) == v; }
        inline bool operator==(const JSON& other) const { return kind == other.kind && value == other.value; }
        inline explicit operator bool() const { return is_bool() && as<bool>(); }

        const JSON& operator[](std::string_view) const;
        const JSON& operator[](size_t) const;
    };

    namespace json {
        static const JSON null = JSON::null();
    }

}

#endif

#endif
