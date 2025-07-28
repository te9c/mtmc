#pragma once

#include <string>
#include <vector>

#include "Season.h"

struct Series {
    std::string name;
    uint32_t yearReleased;
    std::vector<Season> seasons;
};
