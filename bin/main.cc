#include <string_view>
#include <cstdlib>

#include "infinite.h"
#include "util.h"

int main(int argc, char** argv)
{
    if (argc == 1) {
        quit_with_error("No input file specified.");
    }

    infinite::Model* m = infinite::load_model(argv[1]);
    m->predict("haha");

    infinite::free_model(m);

    return EXIT_SUCCESS;
}
