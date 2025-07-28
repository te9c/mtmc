#pragma once

#include <filesystem>

struct Episode {
    uint32_t number;
    std::filesystem::path path_to_media;
};
