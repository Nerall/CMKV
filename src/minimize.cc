#include "minimize.hh"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

namespace {
    const auto kBeta = 0.8;
    const auto kPi = 0.15;
    const auto kGamma = 0.5 * log((1 - kPi) / kPi);
    int kThreshold;
}

namespace cmkv
{

    int sum_neighbours(const image<std::uint8_t> img, int x, int y)
    {
        int sum = 0;
        for (auto j = y - 1; j <= y + 1; j++) {
            for (auto i = x - 1; i != x + 1; i++) {
                if (i == 0 and j == 0)
                    continue;
                if (img.has(i, j))
                    sum += img(i, j) - kThreshold; // 1 or -1
            }
        }

        return sum;
    }

    image<rgb8_t> minimize(const image<std::uint8_t>& img)
    {
        auto output = convert_binary(img, kThreshold);
        for (std::size_t y = 0; y < img.height; ++y) {
            for (std::size_t x = 0; x < img.width; ++x) {
                output(x, y) = img(x, y);
            }
        }

        auto vec = *img.data;
        std::sort(vec.begin(), vec.end());
        kThreshold = int(vec[img.height * img.width / 2]); // Median

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<int> dist_int(0, img.width * img.height - 1);
        std::uniform_real_distribution<> dist_float(0, 1);

        for (std::size_t y = 0; y < img.height; y++)
            for (std::size_t x = 0; x < img.width; x++)
            {
                auto delta = -2. * kGamma * (img(x, y) - kThreshold) * (output(x, y) - kThreshold)
                             -2. * kBeta * (output(x, y) - kThreshold) * sum_neighbours(output, x, y);

                auto r = dist_float(generator);
                if (log(r) < delta)
                    output(x, y) = 0;
                else
                    output(x, y) = 255;
            }

        return convert_rgb(output);
    }

} // cmkv
