#include <memory>
#include <string_view>

#include "infinite.h"
#include "model.h"

namespace infinite {

    std::shared_ptr<Model> load_model(std::string_view path) {
        return std::shared_ptr<Model>(new GGUFModel { path });
    }

}

InfiniteModelHandle infinite_load_model(const char* path) {
    return new infinite::GGUFModel { path };
}

void infinite_free_model(InfiniteModelHandle handle) {
    delete reinterpret_cast<infinite::GGUFModel*>(handle);
}

void infinite_predict(InfiniteModelHandle handle, const char* input) {
    reinterpret_cast<infinite::Model*>(handle)->predict(input);
}
