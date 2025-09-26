#ifndef _HEADER_H_
#define _HEADER_H_

#include <cstdint>

namespace infinite {
    
    class Header
    {
    private:
        uint32_t magic;
        uint32_t version;
        uint64_t tensor_count;
        uint64_t metadata_kv_count;

    public:
        void validate() const;

        uint32_t get_version() const;
        uint64_t get_tensor_count() const;
        uint64_t get_metadata_kv_count() const;
    };

}

#endif