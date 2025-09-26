#ifndef _FILEMAP_H_
#define _FILEMAP_H_

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace infinite {

    class FileMap
    {
    private:
        int fd;
        size_t size;
        void *data;

    public:
        FileMap() = delete;
        FileMap(const FileMap& other) = delete;
        FileMap& operator=(const FileMap& other) = delete;

        FileMap(std::string_view filename);
        FileMap(FileMap&& other);
        FileMap& operator=(FileMap&& other);

        virtual ~FileMap();

        const uint8_t* get_data() const;
    };

}

#endif