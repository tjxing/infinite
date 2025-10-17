#include "string_t.h"

using namespace infinite;

std::tuple<std::string_view, const uint8_t*> infinite::parse_string(const uint8_t* data)
{
    const uint64_t len = *reinterpret_cast<const uint64_t*>(data);
    const uint8_t* _data = data + sizeof(uint64_t);
    std::string_view s { reinterpret_cast<const char*>(_data), len };
    return std::make_tuple(s, _data + len);
}