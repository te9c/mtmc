#pragma once

#include <filesystem>
#include <vector>

#include "Subtitle.h"
#include "Audio.h"

struct MediaItem {
    std::filesystem::path mediaFile;
    std::vector<Subtitle> extSubtitles;
    std::vector<Audio> extAudios;
};
