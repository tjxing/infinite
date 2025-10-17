#include <memory>
#include <string_view>

#include "infinite.h"
#include "model.h"

namespace infinite {

    void Model::predict(std::string_view input) {
        return reinterpret_cast<GGUFModel*>(handle)->predict(input);
    }

    Model::~Model() {
        delete reinterpret_cast<GGUFModel*>(handle);
    }

    std::shared_ptr<Model> Model::load_model(std::string_view path) {
        return std::shared_ptr<Model>(new Model {new GGUFModel { path } });
    }

}

InfiniteModelHandle infinite_load_model(const char* path) {
    return new infinite::GGUFModel { path };
}

void infinite_free_model(InfiniteModelHandle handle) {
    delete reinterpret_cast<infinite::GGUFModel*>(handle);
}

void infinite_predict(InfiniteModelHandle handle, const char* input) {
    reinterpret_cast<infinite::GGUFModel*>(handle)->predict(input);
}
