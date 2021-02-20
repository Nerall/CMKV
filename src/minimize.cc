#include <iostream>
#include "minimize.hh"

namespace cmkv
{
    float sum_neighbours(image<std::uint8_t> output, size_t i, size_t j)
    {
        float sum = 0;
        if (j > 0)
            sum += ((*output.data)[i + (j - 1) * output.width] - 128) / 128;
        if (j < output.width - 1)
            sum += ((*output.data)[i + (j + 1) * output.width] - 128) / 128;
        if (i > 0)
        {
            sum += ((*output.data)[i - 1 + j * output.width] - 128) / 128;
            if (j > 0)
                sum += ((*output.data)[i - 1 + (j - 1) * output.width] - 128) / 128;
            if (j < output.width - 1)
                sum += ((*output.data)[i - 1 + (j + 1) * output.width] - 128) / 128;
        }
        if (i < output.height - 1)
        {
            sum += ((*output.data)[i + 1 + j * output.width] - 128) / 128;
            if (j > 0)
                sum += ((*output.data)[i + 1 + (j - 1) * output.width] - 128) / 128;
            if (j < output.width - 1)
                sum += ((*output.data)[i + 1 + (j + 1) * output.width] - 128) / 128;
        }

        return sum;
    }

    image<std::uint8_t> minimize(const image<std::uint8_t>& img)
    {
        auto beta = 0.8;
        auto pi = 0.15;
        auto gamma = 0.5*log((1 - pi) / pi);
        auto T = 10000U;

        auto output = image<std::uint8_t>(img.width, img.height);
        for (std::size_t y = 0; y < img.height; y++)
            for (std::size_t x = 0; x < img.width; x++)
            {
                (*img.data)[x + y * img.width] = (*img.data)[x + y * img.width] > 128 ? 255 : 0;
                (*output.data)[x + y * img.width] = (*img.data)[x + y * img.width];
            }

        for (std::size_t t = 0; t < T; t++)
        {
            size_t i = rand() % img.height;
            size_t j = rand() % img.width;
            
            float delta = - 2 * gamma * (float)(*img.data)[i + j * img.width] / 255 * (float)(*output.data)[i + j * img.width] / 255
                          - 2 * beta * (float)(*output.data)[i + j * img.width] / 255 * sum_neighbours(output, i, j);

            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

            if (log(r) < delta)
            {
                (*output.data)[i + j * img.width] = (*output.data)[i + j * img.width] ? 0 : 255;
            }
        }

        return output;
    }

} // cmkv