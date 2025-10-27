#include <cstddef>
#include <string_view>
#include <unordered_map>

#include "json.h"

namespace infinite {

    const JSON& JSON::operator[](std::string_view key) const 
    {
        if (!is_object()) {
            return json::null;
        }
        auto& m = *std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
        auto it = m.find(key);
        if (it == m.end()) {
            return json::null;
        }
        return it->second;
    }

    const JSON& JSON::operator[](size_t i) const 
    {
        if (!is_array()) {
            return json::null;
        }
        auto& v = *std::get_if<std::vector<JSON>>(&value);
        if (i < 0 || i >= v.size()) {
            return json::null;
        }
        return v[i];
    }

}

////////////////////////////////////////


