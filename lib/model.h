#ifndef _MODEL_H_
#define _MODEL_H_

#include <string_view>
#include <map>

#include "filemap.h"
#include "header.h"
#include "infinite.h"
#include "metadata.h"

namespace infinite {

    class GGUFModel : public Model
    {
    private:
        FileMap filemap;
        const Header *header;
        std::map<std::string_view, Metadata> metadata;

    public:
        GGUFModel(std::string_view filename);

        virtual void predict(std::string_view);
    };

}
#endif