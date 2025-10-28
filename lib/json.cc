#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "json.h"

namespace infinite {

    JSON JSON::array(std::initializer_list<JSON> data) {
        JSON result = JSON(0);
        result.kind = JSONKind::ARRAY;
        result.value = std::vector<JSON>(data);
        return result;
    }

    JSON JSON::null() {
        JSON result = JSON(0);
        result.kind = JSONKind::EMPTY;
        return result;
    }

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

    size_t JSON::size() const {
        switch (kind) {
        case JSONKind::OBJECT:
            return std::get_if<std::unordered_map<std::string_view, JSON>>(&value)->size();
        case JSONKind::ARRAY:
            return std::get_if<std::vector<JSON>>(&value)->size();
        case JSONKind::EMPTY:
            return 0;
        default:
            return 1;
        }
    }

    bool JSON::add(const JSON& data) {
        if (is_array()) {
            auto v = std::get_if<std::vector<JSON>>(&value);
            v->push_back(data);
            return true;
        }
        return false;
    }

    bool JSON::add(JSON&& data) {
        if (is_array()) {
            auto v = std::get_if<std::vector<JSON>>(&value);
            v->push_back(data);
            return true;
        }
        return false;
    }

    bool JSON::add(std::initializer_list<JSON> data) {
        if (is_array()) {
            auto v = std::get_if<std::vector<JSON>>(&value);
            for (const JSON& j : data) {
                v->push_back(j);
            }
            return true;
        }
        return false;
    }

    bool JSON::add(std::string_view key, const JSON& v) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            o->try_emplace(key, v);
            return true;
        }
        return false;
    }

    bool JSON::add(std::string_view key, JSON&& v) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            o->try_emplace(key, v);
            return true;
        }
        return false;
    }

    bool JSON::add(std::initializer_list<std::pair<std::string_view, JSON>> data) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            for (const auto& pair : data) {
                o->insert(pair);
            }
            return true;
        }
        return false;
    }

    bool JSON::add_or_replace(std::string_view key, const JSON& v) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            o->insert_or_assign(key, v);
            return true;
        }
        return false;
    }

    bool JSON::add_or_replace(std::string_view key, JSON&& v) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            o->insert_or_assign(key, v);
            return true;
        }
        return false;
    }

    bool JSON::add_or_replace(std::initializer_list<std::pair<std::string_view, JSON>> data) {
        if (is_object()) {
            auto o = std::get_if<std::unordered_map<std::string_view, JSON>>(&value);
            for (const auto& pair : data) {
                o->insert_or_assign(pair.first, pair.second);
            }
            return true;
        }
        return false;
    }

}

////////////////////////////////////////

InfiniteJSONObjectHandle infinite_make_json_object() {
    return new infinite::JSON {};
}

int infinite_json_object_add_int(InfiniteJSONObjectHandle handle, const char* key, int i) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, i);
}

int infinite_json_object_add_double(InfiniteJSONObjectHandle handle, const char* key, double d) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, d);
}

int infinite_json_object_add_bool(InfiniteJSONObjectHandle handle, const char* key, int b) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, static_cast<bool>(b));
}

int infinite_json_object_add_string(InfiniteJSONObjectHandle handle, const char* key, const char* s) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, s);
}

int infinite_json_object_add_object(InfiniteJSONObjectHandle handle, const char* key, InfiniteJSONObjectHandle o) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, *reinterpret_cast<infinite::JSON*>(o));
}

int infinite_json_object_add_array(InfiniteJSONObjectHandle handle, const char* key, InfiniteJSONArrayHandle a) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, *reinterpret_cast<infinite::JSON*>(a));
}

int infinite_json_object_add_null(InfiniteJSONArrayHandle handle, const char* key) {
    return reinterpret_cast<infinite::JSON*>(handle)->add_or_replace(key, infinite::json::null);
}

InfiniteJSONArrayHandle infinite_make_json_array() {
    infinite::JSON* result = new infinite::JSON(0);
    *result = infinite::JSON::array();
    return result;
}

int infinite_json_array_add_int(InfiniteJSONArrayHandle handle, int i) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(i);
}

int infinite_json_array_add_double(InfiniteJSONArrayHandle handle, double d) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(d);
}

int infinite_json_array_add_bool(InfiniteJSONArrayHandle handle, int b) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(static_cast<bool>(b));
}

int infinite_json_array_add_string(InfiniteJSONArrayHandle handle, const char* s) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(s);
}

int infinite_json_array_add_object(InfiniteJSONArrayHandle handle, InfiniteJSONObjectHandle o) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(*reinterpret_cast<infinite::JSON*>(o));
}

int infinite_json_array_add_array(InfiniteJSONArrayHandle handle, InfiniteJSONArrayHandle a) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(*reinterpret_cast<infinite::JSON*>(a));
}

int infinite_json_array_add_null(InfiniteJSONArrayHandle handle) {
    return reinterpret_cast<infinite::JSON*>(handle)->add(infinite::json::null);
}

void infinite_free_json(InfiniteJSONHandle handle) {
    delete reinterpret_cast<infinite::JSON*>(handle);
}
