#ifndef _COMMON_H_
#define _COMMON_H_

#include <bit>
#include <cstdlib>
#include <cstdint>

namespace infinite {

constexpr bool is_little_endian = std::endian::native == std::endian::little;
constexpr bool is_big_endian = !is_little_endian;

inline uint32_t byte_swap32(uint32_t n) {
    #if defined (__GNUC__) || defined (__clang__)
        return __builtin_bswap32(n);
    #else
        return ((n & 0xFF000000) >> 24) |
            ((n & 0x00FF0000) >> 8)     |
            ((n & 0x0000FF00) << 8)     |
            ((n & 0x000000FF) << 24);
    #endif
}

inline uint64_t byte_swap64(uint64_t n) {
    #if defined (__GNUC__) || defined (__clang__)
        return __builtin_bswap64(n);
    #else
        return ((n & 0xFF00000000000000) >> 56) |
            ((n & 0x00FF000000000000) >> 40)    |
            ((n & 0x0000FF0000000000) >> 24)    |
            ((n & 0x000000FF00000000) >> 8)     |
            ((n & 0x00000000FF000000) << 8)     |
            ((n & 0x0000000000FF0000) << 24)    |
            ((n & 0x000000000000FF00) << 40)    |
            ((n & 0x00000000000000FF) << 56);
    #endif
}

inline uint32_t from_little_endian(uint32_t n) {
    if constexpr (is_little_endian) {
        return n;
    } else {
        return byte_swap32(n);
    }
}

inline uint64_t from_little_endian(uint64_t n) {
    if constexpr (is_little_endian) {
        return n;
    } else {
        return byte_swap64(n);
    }
}

}

#endif