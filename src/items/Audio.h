#pragma once

#include <filesystem>
#include <string>

struct Audio {
    std::string language;
    std::string author;
    std::filesystem::path path;
};
