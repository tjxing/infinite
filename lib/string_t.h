#ifndef _STRING_T_H_
#define _STRING_T_H_

#include <string_view>
#include <tuple>
#include <cstdint>

namespace infinite {

    std::tuple<std::string_view, const uint8_t*> parse_string(const uint8_t*);
    
}

#endif