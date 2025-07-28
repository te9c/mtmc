#pragma once

#include <future>
#include <filesystem>

#include "TorrentFileLink.h"

class Downloader {
public:
    virtual std::future<std::filesystem::path> DownloadTorrent(TorrentFileLink torrent) = 0;
};
