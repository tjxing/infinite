#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

#include "header.h"

class GGUFTest : public testing::Test 
{
protected:
    char* data;

    GGUFTest()
    {
        std::fstream f;
        f.open("example.gguf", std::ios::in | std::ios::binary | std::ios::ate);
        auto size = f.tellg();
        this->data = new char[size];

        f.seekg(0);
        f.read(this->data, size);
    }

    virtual ~GGUFTest() { delete[] data; }
};

TEST_F(GGUFTest, Header) {
    infinite::Header* header = reinterpret_cast<infinite::Header*>(data);
    ASSERT_EQ(header->get_version(), 3);
    ASSERT_EQ(header->get_metadata_kv_count(), 5);
}
