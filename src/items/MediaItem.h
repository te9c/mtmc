#pragma once

#include <cstdint>
#include <string>

struct MediaItem {
    std::string name;
    uint32_t year_released;

    virtual std::string GetFullName() {
        return name + "(" + std::to_string(year_released) + ")";
    };
};
