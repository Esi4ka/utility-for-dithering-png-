#pragma once

#include "kd_tree.h"
#include <image.h>
#include <vector>
#include <cmath>
#include <cstdlib>

std::array<double, 3> operator+(const std::array<double, 3>& lhs,
                                const std::array<double, 3>& rhs) {
    std::array<double, 3> result;
    result[0] = lhs[0] + rhs[0];
    result[1] = lhs[1] + rhs[1];
    result[2] = lhs[2] + rhs[2];
    return result;
}

std::array<double, 3> operator-(const std::array<double, 3>& lhs,
                                const std::array<double, 3>& rhs) {
    std::array<double, 3> result;
    result[0] = lhs[0] - rhs[0];
    result[1] = lhs[1] - rhs[1];
    result[2] = lhs[2] - rhs[2];
    return result;
}

std::array<double, 3> operator*(const std::array<double, 3>& lhs, double val) {
    std::array<double, 3> result;
    result[0] = lhs[0] * val;
    result[1] = lhs[1] * val;
    result[2] = lhs[2] * val;
    return result;
}

std::array<double, 3> CastFunc(const RGB& value) {
    std::array<double, 3> result;
    result[0] = static_cast<double>(value.r);
    result[1] = static_cast<double>(value.g);
    result[2] = static_cast<double>(value.b);
    return result;
}

RGB ReverseCastFunc(const std::array<double, 3>& value) {
    RGB result;
    result.r = value[0];
    result.g = value[1];
    result.b = value[2];
    return result;
}

Image Dither(const Image& input, const std::vector<RGB>& colors) {
    std::vector<std::array<double, 3>> prepared_colors(colors.size());

    int ind = 0;
    for (const auto& el : colors) {
        prepared_colors[ind] = CastFunc(el);
        ++ind;
    }

    KdTree new_tree(prepared_colors);

    int height = input.Height();
    int width = input.Width();
    Image tmp_image(width, height);

    std::vector<std::vector<Point<3>>> matrix;
    matrix.resize(height);
    for (int h = 0; h < height; ++h) {
        matrix[h].resize(width);
        for (int w = 0; w < width; ++w) {
            RGB pixel_value = input.GetPixel(h, w);
            matrix[h][w] = CastFunc(pixel_value);
        }
    }

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            auto old_pixel = matrix[h][w];
            auto new_pixel_index = new_tree.GetNearest(old_pixel);
            auto new_pixel_value = prepared_colors[new_pixel_index];
            matrix[h][w] = new_pixel_value;

            std::array<double, 3> error;
            error = old_pixel - new_pixel_value;

            if (w + 1 < width) {
                std::array<double, 3> pixel = matrix[h][w + 1];
                std::array<double, 3> res_val = pixel + (error * (7.f / 16.));
                matrix[h][w + 1] = res_val;
            }

            if (w > 0 && h + 1 < height) {
                auto pixel = matrix[h + 1][w - 1];
                matrix[h + 1][w - 1] = pixel + (error * (3.f / 16.));
            }

            if (h + 1 < height) {
                auto pixel = matrix[h + 1][w];
                matrix[h + 1][w] = pixel + (error * (5.f / 16.));
            }

            if (h + 1 < height && w + 1 < width) {
                auto pixel = matrix[h + 1][w + 1];
                matrix[h + 1][w + 1] = pixel + (error * (1.f / 16.));
            }
        }
    }

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            auto old_pixel = matrix[h][w];
            RGB new_pixel = ReverseCastFunc(old_pixel);
            tmp_image.SetPixel(new_pixel, h, w);
        }
    }
    return tmp_image;
}
