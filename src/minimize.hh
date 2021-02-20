#ifndef MINIMIZE_HH
#define MINIMIZE_HH

#include <math.h>
#include <random>
#include "image.hh"

namespace cmkv
{
  image<std::uint8_t> minimize(const image<std::uint8_t>& img);
}

#endif
