#include <cstdint>
#include <stdexcept>

#include "header.h"
#include "common.h"

using namespace infinite;

void Header::validate() const 
{
    constexpr uint32_t correct_magic = is_little_endian ? 0x46554747 :  0x47475546;
    if (this->magic != correct_magic) {
        throw std::runtime_error("Malformed GGUF file.");
    }
    if (this->version > 3) {
        throw std::runtime_error("Unsupported GGUF version.");
    }
}

uint32_t Header::get_version() const
{
    return from_little_endian(this->version);
}

uint64_t Header::get_tensor_count() const
{
    return from_little_endian(this->tensor_count);
}

uint64_t Header::get_metadata_kv_count() const
{
    return from_little_endian(this->metadata_kv_count);
}