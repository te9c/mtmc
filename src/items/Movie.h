#pragma once

#include <filesystem>

#include "MediaItem.h"

struct Movie : public MediaItem {
    std::filesystem::path path_to_movie;
};
