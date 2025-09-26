#include <cstdint>
#include <string_view>
#include <utility>
#include <iostream>

#include "model.h"
#include "filemap.h"
#include "header.h"
#include "metadata.h"

using namespace infinite;

GGUFModel::GGUFModel(std::string_view filename) : filemap(filename)
{
    const uint8_t* data = this->filemap.get_data();
    this->header = reinterpret_cast<const Header*>(data);
    this->header->validate();

    data = data + sizeof(Header);
    for (int i = 0; i < this->header->get_metadata_kv_count(); ++i)
    {
        const auto [key, next] = parse_string(data);
        const auto [metadata, next_next] = parse_metadata(next);
        this->metadata.emplace(std::make_pair(key, metadata));
        data = next_next;
    }
}

void GGUFModel::predict(std::string_view input)
{
    std::cout << input << " ! " << std::endl;
}
