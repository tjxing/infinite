#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <cstdint>
#include <tuple>
#include <vector>

namespace infinite {

    class Tensor
    {
    private:
        std::vector<uint64_t> dims;
        uint32_t type;
        uint64_t offset;
        const uint8_t *data;
    
    public:
        Tensor(uint32_t dim_count, const uint64_t* dim_data, uint32_t type, uint64_t offset);

        void adjust(const uint8_t *data);
        inline long get_data() const {
            return (long) this->data;
        }
    };

    std::tuple<Tensor, const uint8_t*> parse_tensor(const uint8_t*);

}

#endif