#ifndef MINIMIZE_HH
#define MINIMIZE_HH

#include "image.hh"

namespace cmkv
{
    int sum_neighbours(const image<std::uint8_t> img, int x, int y);
    image<rgb8_t> minimize(const image<std::uint8_t>& img);
}

#endif
