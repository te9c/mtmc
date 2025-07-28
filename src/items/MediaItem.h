#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include "Subtitle.h"
#include "Audio.h"

struct MediaItem {
    std::filesystem::path mediaFile;
    std::vector<Subtitle> ext_subtitles;
    std::vector<Audio> ext_audios;
};
