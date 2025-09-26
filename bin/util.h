#include <iostream>

inline void quit_with_error(const char *error)
{
    std::cerr << error << std::endl;
    std::exit(EXIT_FAILURE);
}