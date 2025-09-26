#ifndef _INFINITE_H_
#define _INFINITE_H_

#include <memory>
#ifdef __cplusplus

#include <string_view>

namespace infinite {

    class Model
    {
    public:
        virtual void predict(std::string_view input) = 0;

        virtual ~Model() {}
    };

    std::shared_ptr<Model> load_model(std::string_view path);

}

extern "C" {
#endif

typedef void* InfiniteModelHandle;

InfiniteModelHandle infinite_load_model(const char* path);
void infinite_free_model(InfiniteModelHandle handle);

void infinite_predict(InfiniteModelHandle handle, const char* input);

#ifdef __cplusplus
}
#endif

#endif
