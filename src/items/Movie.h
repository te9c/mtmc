#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "MediaItem.h"

struct Movie {
    std::string name;
    uint32_t yearReleased;
    MeidaItem movie;
};
