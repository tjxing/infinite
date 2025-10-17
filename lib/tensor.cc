#include <cstdint>
#include <tuple>
#include <vector>

#include "tensor.h"

using namespace infinite;

Tensor::Tensor(uint32_t dim_count, const uint64_t* dim_data, uint32_t type, const uint64_t offset)
    : dims(std::vector<uint64_t>()), type(type), offset(offset), data(nullptr)
{
    for (int i = 0; i < dim_count; ++i)
    {
        this->dims.push_back(*dim_data);
        ++dim_data;
    }
}

void Tensor::adjust(const uint8_t *data) {
    this->data = data + this->offset;
}

std::tuple<Tensor, const uint8_t*> infinite::parse_tensor(const uint8_t* data)
{
    uint32_t dim_count = *reinterpret_cast<const uint32_t*>(data);
    data += sizeof(uint32_t);
    
    const uint64_t* dim_data = reinterpret_cast<const uint64_t*>(data);
    data += dim_count * sizeof(uint64_t);

    uint32_t type = *reinterpret_cast<const uint32_t*>(data);
    data += sizeof(uint32_t);

    uint64_t offset = *reinterpret_cast<const uint64_t*>(data);
    data += sizeof(uint64_t);

    return std::make_tuple(Tensor(dim_count, dim_data, type, offset), data);
}