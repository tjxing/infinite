#include <string_view>

#include "infinite.h"
#include "model.h"

namespace infinite {

    Model* load_model(std::string_view path) {
        return new GGUFModel { path };
    }

    void free_model(Model* model) {
        delete model;
    }

}

InfiniteModelHandle infinite_load_model(const char* path) {
    return infinite::load_model(std::string_view { path });
}

void infinite_free_model(InfiniteModelHandle handle) {
    infinite::free_model(reinterpret_cast<infinite::Model*>(handle));
}

void infinite_predict(InfiniteModelHandle handle, const char* input) {
    reinterpret_cast<infinite::Model*>(handle)->predict(input);
}
