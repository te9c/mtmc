#pragma once

#include <vector>

#include "MediaItem.h"
#include "Season.h"

struct Series : public MediaItem {
    std::vector<Season> seasons;
};
