#include <stdexcept>
#include <string_view>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "filemap.h"
#include "common.h"

using namespace infinite;

FileMap::FileMap(std::string_view filename)
{
    this->fd = open(filename.data(), O_RDONLY);
    if (fd < 0) {
        throw std::runtime_error("Failed to open input file.");
    }

    struct stat stat;
    if (fstat(this->fd, &stat) < 0) {
        throw std::runtime_error("Failed to get stat from input file.");
    }
    this->size = stat.st_size;

    this->data = mmap(NULL, size, PROT_READ, MAP_SHARED, this->fd, 0);
    if (this->data == MAP_FAILED) {
        throw std::runtime_error("Failed to read input file.");
    }
}

FileMap::FileMap(FileMap&& other) : fd(other.fd), size(other.size), data(other.data)
{
    other.fd = -1;
    other.size = -1;
    other.data = nullptr;
}

FileMap& FileMap::operator=(FileMap&& other)
{
    this->fd = other.fd;
    this->size = other.size;
    this->data = other.data;

    other.fd = -1;
    other.size = -1;
    other.data = nullptr;

    return *this;
}

FileMap::~FileMap()
{
    if (this->data != nullptr) {
        munmap(this->data, this->size);
        close(this->fd);
    }
}

const uint8_t* FileMap::get_data() const
{
    return static_cast<const uint8_t*>(this->data);
}
