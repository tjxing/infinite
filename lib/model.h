#ifndef _MODEL_H_
#define _MODEL_H_

#include <string_view>
#include <unordered_map>

#include "filemap.h"
#include "header.h"
#include "metadata.h"
#include "tensor.h"

namespace infinite {

    class GGUFModel
    {
    private:
        FileMap filemap;
        const Header *header;
        std::unordered_map<std::string_view, Metadata> metadata;
        std::unordered_map<std::string_view, Tensor> tensor;
        const uint8_t *data;

    public:
        GGUFModel(std::string_view filename);

        void predict(std::string_view);
    };

}
#endif