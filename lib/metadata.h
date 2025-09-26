#ifndef _METADATA_H_
#define _METADATA_H_

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string_view>
#include <tuple>
#include <variant>

namespace infinite {

enum class MetadataValueType
{
    GGUF_METADATA_VALUE_TYPE_UINT8,
    GGUF_METADATA_VALUE_TYPE_INT8,
    GGUF_METADATA_VALUE_TYPE_UINT16,
    GGUF_METADATA_VALUE_TYPE_INT16,
    GGUF_METADATA_VALUE_TYPE_UINT32,
    GGUF_METADATA_VALUE_TYPE_INT32,
    GGUF_METADATA_VALUE_TYPE_FLOAT32,
    GGUF_METADATA_VALUE_TYPE_BOOL,
    GGUF_METADATA_VALUE_TYPE_STRING,
    GGUF_METADATA_VALUE_TYPE_ARRAY,
    GGUF_METADATA_VALUE_TYPE_UINT64,
    GGUF_METADATA_VALUE_TYPE_INT64,
    GGUF_METADATA_VALUE_TYPE_FLOAT64
};

class MetadataArray;

template<typename T>
concept MetadataSimpleType = 
    std::same_as<T, uint8_t> ||
    std::same_as<T, int8_t> ||
    std::same_as<T, uint16_t> ||
    std::same_as<T, int16_t> ||
    std::same_as<T, uint32_t> ||
    std::same_as<T, int32_t> ||
    std::same_as<T, float> ||
    std::same_as<T, bool> ||
    std::same_as<T, uint64_t> ||
    std::same_as<T, int64_t> ||
    std::same_as<T, double>;

template<typename T>
concept MetadataType = 
    MetadataSimpleType<T> || 
    std::same_as<T, std::string_view> || 
    std::same_as<T, MetadataArray>;

template<MetadataSimpleType T>
std::tuple<T, const uint8_t*> parse_simple_value(const uint8_t* data)
{
    const T x = *reinterpret_cast<const T*>(data);
    return std::make_tuple(x, data + sizeof(T));
}
std::tuple<std::string_view, const uint8_t*> parse_string(const uint8_t*);
std::tuple<MetadataArray, const uint8_t*> parse_array(const uint8_t*);

template<MetadataType T> class MetadataIterable;

class MetadataArray
{
private:
    MetadataValueType type;
    uint64_t len;
    const uint8_t* data;

public:
    MetadataArray() = default;
    MetadataArray(MetadataValueType type, uint64_t len, const uint8_t* data);

    template<MetadataType T>
    MetadataIterable<T> iter() {
        return MetadataIterable<T>(this->len, this->data);
    }

    inline MetadataValueType get_type() { return type; }
    inline uint64_t get_length() { return len; }
};

template<MetadataType T>
class MetadataIterable
{
private:
    uint64_t len;
    const uint8_t* data;

public:
    class Iterator
    {
    private:
        uint64_t max;
        uint64_t current;
        const uint8_t* data;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        Iterator(uint64_t max, uint64_t current, const uint8_t* data)
            : max(max), current(current), data(data) {}

        const T& operator*() const
        {
            if constexpr (MetadataSimpleType<T>) {
                const auto [v, _] = parse_simple_value<T>(this->data);
                return v;
            } else if constexpr (std::same_as<T, std::string_view>) {
                const auto [v, _] = parse_string(this->data);
                return v;
            } else {
                const auto [v, _] = parse_array(this->data);
                return v;
            }
        }
        Iterator& operator++()
        {
            ++this->current;
            if constexpr (MetadataSimpleType<T>) {
                const auto [_, next] = parse_simple_value<T>(this->data);
                this->data = this->current >= this->max ? nullptr : next;
            } else if constexpr (std::same_as<T, std::string_view>) {
                const auto [_, next] = parse_string(this->data);
                this->data = this->current >= this->max ? nullptr : next;
            } else {
                const auto [_, next] = parse_array(this->data);
                this->data = this->current >= this->max ? nullptr : next;
            }
            return *this;
        }
        Iterator& operator++(int)
        {
            MetadataIterable<T>::Iterator& temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const Iterator& other) const { return this->data == other.data; }
    };

    MetadataIterable(uint64_t len, const uint8_t* data) : len(len), data(data) {}
    Iterator begin() { return MetadataIterable<T>::Iterator { len, 0, data }; }
    Iterator end() { return MetadataIterable<T>::Iterator { len, len, nullptr }; }
};

using MetadataValue = std::variant<
    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, float, bool, 
    std::string_view, MetadataArray, uint64_t, 
    int64_t, double>;

class Metadata
{
private:
    MetadataValueType type;
    MetadataValue value;

public:
    Metadata(MetadataValueType type, MetadataValue value);

    inline MetadataValueType get_type() const { return this->type;}

    template<MetadataType T>
    inline const T& get_value() const { return *std::get_if<T>(&this->value); }
};

std::tuple<Metadata, const uint8_t*> parse_metadata(const uint8_t*);

}

#endif
