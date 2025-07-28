#pragma once

#include <cstdint>
#include <vector>

#include "Episode.h"

struct Season {
    uint32_t number;
    std::vector<Episode> episodes;
};
