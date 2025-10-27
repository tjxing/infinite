#include <cstdint>
#include <tuple>

#include "metadata.h"

using namespace infinite;

const uint8_t* skip_array(MetadataValueType type, const uint64_t len, const uint8_t* data)
{
    const uint8_t* next = data;
    switch (type)
    {
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT8: 
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT8: 
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_BOOL:
        next += len * sizeof(uint8_t);
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT16:
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT16:
        next += len * sizeof(uint16_t);
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT32:
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT32:
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT32:
        next += len * sizeof(uint32_t);
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT64:
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT64:
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT64:
        next += len * sizeof(uint64_t);
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_STRING:
        for (int i = 0; i < len; ++i) {
            const uint64_t _len = *reinterpret_cast<const uint64_t*>(next);
            next += sizeof(uint64_t) + _len;
        }
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_ARRAY:
        for (int i = 0; i < len; ++i) {
            const uint32_t t = *reinterpret_cast<const uint32_t*>(next);
            MetadataValueType _type = static_cast<MetadataValueType>(t);
            const uint64_t _len = *reinterpret_cast<const uint64_t*>(next + sizeof(uint32_t));
            next = skip_array(_type, _len, next + sizeof(uint32_t) + sizeof(uint64_t));
        }
        break;
    }
    return next;
}

std::tuple<MetadataArray, const uint8_t*> infinite::parse_array(const uint8_t* data)
{
    const uint32_t t = *reinterpret_cast<const uint32_t*>(data);
    MetadataValueType type = static_cast<MetadataValueType>(t);
    const uint64_t len = *reinterpret_cast<const uint64_t*>(data + sizeof(uint32_t));

    const uint8_t* _data = data + sizeof(uint32_t) + sizeof(uint64_t);
    MetadataArray a { type, len, _data };
    return std::make_tuple(a, skip_array(type, len, _data));
}

std::ostream& infinite::operator<<(std::ostream& os, const Metadata& metadata)
{
    switch (metadata.get_type()) {
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT8:
        os << metadata.get_value<uint8_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT8:
        os << metadata.get_value<int8_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT16:
        os << metadata.get_value<uint16_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT16:
        os << metadata.get_value<int16_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT32:
        os << metadata.get_value<uint32_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT32:
        os << metadata.get_value<int32_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT32:
        os << metadata.get_value<float>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_BOOL:
        os << metadata.get_value<bool>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_STRING:
        os << metadata.get_value<std::string_view>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_ARRAY:
        os << "[array]";
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT64:
        os << metadata.get_value<uint64_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT64:
        os << metadata.get_value<int64_t>();
        break;
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT64:
        os << metadata.get_value<double>();
        break;
    }
    return os;
}

std::tuple<Metadata, const uint8_t*> infinite::parse_metadata(const uint8_t* data)
{
    const uint32_t t = *reinterpret_cast<const uint32_t*>(data);
    MetadataValueType type = static_cast<MetadataValueType>(t);
    data = data + sizeof(uint32_t);

    switch (type)
    {
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT8: {
        const auto [i, next] = parse_simple_value<uint8_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT8: {
        const auto [i, next] = parse_simple_value<int8_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT16: {
        const auto [i, next] = parse_simple_value<uint16_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT16: {
        const auto [i, next] = parse_simple_value<int16_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT32: {
        const auto [i, next] = parse_simple_value<uint32_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT32: {
        const auto [i, next] = parse_simple_value<int32_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT32: {
        const auto [f, next] = parse_simple_value<float>(data);
        return std::make_tuple(Metadata { type, f }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_BOOL: {
        const auto [b, next] = parse_simple_value<bool>(data);
        return std::make_tuple(Metadata { type, b }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_STRING: {
        const auto [s, next] = parse_string(data);
        return std::make_tuple(Metadata { type, s }, next);
        break;
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_UINT64: {
        const auto [i, next] = parse_simple_value<uint64_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_INT64: {
        const auto [i, next] = parse_simple_value<int64_t>(data);
        return std::make_tuple(Metadata { type, i }, next);
    }
    case MetadataValueType::GGUF_METADATA_VALUE_TYPE_FLOAT64: {
        const auto [f, next] = parse_simple_value<double>(data);
        return std::make_tuple(Metadata { type, f }, next);
    }
    default:
        const auto [a, next] = parse_array(data);
        return std::make_tuple(Metadata { type, a }, next);
    }
}
