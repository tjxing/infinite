#include <memory>
#include <string_view>
#include <cstdlib>

#include "infinite.h"
#include "util.h"

int main(int argc, char** argv)
{
    if (argc == 1) {
        quit_with_error("No input file specified.");
    }

    std::shared_ptr<infinite::Model> m = infinite::Model::load_model(argv[1]);
    m->predict("haha");

    return EXIT_SUCCESS;
}
