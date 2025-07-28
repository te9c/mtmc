#pragma once

#include <filesystem>
#include <string>

struct Subtitle {
    std::filesystem::path path;
    std::string language;
    std::string author;
    bool forced;
};
