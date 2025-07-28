#pragma once

#include <cstdint>

#include "MediaItem.h"

struct Episode {
    uint32_t number;
    MediaItem media;
};
