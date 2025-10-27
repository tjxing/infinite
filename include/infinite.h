#ifndef _INFINITE_H_
#define _INFINITE_H_

#include "json.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* InfiniteModelHandle;

InfiniteModelHandle infinite_load_model(const char* path);
void infinite_free_model(InfiniteModelHandle handle);

void infinite_predict(InfiniteModelHandle handle, const char* input);

////////////////////////////////////////

#ifdef __cplusplus
}

#include <memory>
#include <string_view>

namespace infinite {

    class Model
    {
    private:
        InfiniteModelHandle handle;
        Model(InfiniteModelHandle handle): handle(handle) {}
    public:
        Model(Model&) = delete;
        Model& operator=(const Model&) = delete;

        Model(Model&&);
        Model& operator=(Model&&);
        virtual ~Model();

        void predict(std::string_view input);

        static std::shared_ptr<Model> load_model(std::string_view path);
    };

}
#endif

#endif
