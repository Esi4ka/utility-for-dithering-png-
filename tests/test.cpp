#include "kd_tree.h"
#include "dither.h"
#include <util.h>
#include <image.h>

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include <filesystem>
#include <ranges>

#include <catch2/catch_test_macros.hpp>

namespace {

    const auto kDir = GetFileDir(__FILE__);

    TEST_CASE("nigga") {
        auto image = Dither(Image{kDir / "test_im.png"}, {{0,   0,   0},
                                                          {255, 255, 255}});
        image.Write(std::filesystem::path(kDir / "my_nigga.png"));
        // CheckPixels(image, Image{kDir / "2_result.png"});
    }

    TEST_CASE("trump") {
        auto image = Dither(Image{kDir / "trump.png"}, {{0,   0,   0},
                                                          {255, 255, 255}});
        image.Write(std::filesystem::path(kDir / "trump_nigga.png"));
        // CheckPixels(image, Image{kDir / "2_result.png"});
    }

}
