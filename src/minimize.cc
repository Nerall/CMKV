#include "minimize.hh"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

namespace cmkv
{


    int sum_neighbours(const image<std::uint8_t> img, int x, int y)
    {
        int sum = 0;
        for (auto j = y - 1; j <= y + 1; j++) {
            for (auto i = x - 1; i <= x + 1; i++) {
                if (i == 0 and j == 0)
                    continue;
                if (img.has(i, j))
                    sum += img(i, j) * 2 - 1; // 1 or -1
            }
        }

        return sum;
    }

    image<rgb8_t> minimize(const image<std::uint8_t>& img)
    {
        auto kBeta = 0.8;
        auto kPi = 0.15;
        auto kGamma = 0.5 * log((1 - kPi) / kPi);
        auto kT = img.width * img.height * 6;
        auto kThreshold = 128;

        auto output = convert_binary(img, kThreshold);

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<int> dist_int(0, img.width * img.height - 1);
        std::uniform_real_distribution<> dist_float(0, 1);

        for (auto t = 0U; t < kT; t++) {
            auto rand_int = dist_int(generator);
            int x = rand_int % img.width;
            int y = rand_int / img.width;
            auto delta = -2. * kGamma * ((img(x, y) >= kThreshold) * 2 - 1) * (output(x, y) * 2 - 1)
                         -2. * kBeta * (output(x, y) * 2 - 1) * sum_neighbours(output, x, y);

            auto r = dist_float(generator);
            if (log(r) < delta)
                output(x, y) = !output(x, y);
        }

        return binary_to_rgb(output);
    }

} // cmkv
