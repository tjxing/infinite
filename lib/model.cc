#include <cstdint>
#include <string_view>
#include <iostream>

#include "model.h"
#include "filemap.h"
#include "header.h"
#include "string_t.h"
#include "metadata.h"
#include "tensor.h"

using namespace infinite;

GGUFModel::GGUFModel(std::string_view filename): filemap(filename)
{
    const uint8_t* data = this->filemap.get_data();
    this->header = reinterpret_cast<const Header*>(data);
    this->header->validate();

    data = data + sizeof(Header);
    for (int i = 0; i < this->header->get_metadata_kv_count(); ++i)
    {
        const auto [key, next] = parse_string(data);
        const auto [metadata, next_next] = parse_metadata(next);
        this->metadata.try_emplace(key, metadata);
        data = next_next;

        std::cout << key << " : " << metadata << std::endl;
    }

    for (int i = 0; i < this->header->get_tensor_count(); ++i)
    {
        const auto [name, next] = parse_string(data);
        const auto [tensor, next_next] = parse_tensor(next);
        this->tensor.try_emplace(name, std::move(tensor));
        data = next_next;
    }
    for (auto& pair : this->tensor) {
        pair.second.adjust(data);
    }

    this->data = data;
}

void GGUFModel::predict(std::string_view input)
{
    std::cout << input << " ! " << std::endl;
}
