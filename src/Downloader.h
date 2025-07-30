// This file contains interface of a torrent client.
// The puprose of this is to make one
// unified interface for any torrent client.
// Implementations for specific torrent clients
// is located under downloaders directory.
// Keep in mind that this is not a download
// controller or manager.

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>

#include "TorrentFileLink.h"

struct TorrentDownloadRequest {
    std::filesystem::path torrentFilePath;
    std::filesystem::path savePath;
    bool paused = true;
    std::vector<std::string> files; // empty for all
};

struct TorrentDownloadingInfo {
    std::string name;
    std::string hash;
    std::filesystem::path savedPath;
    uint64_t downloadedBytes;
    uint64_t totalSizeBytes;
};

struct DownloaderError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Downloader {
public:
    virtual void AddTorrent(const TorrentDownloadRequest& request) = 0;
    virtual void DeleteTorrent(const std::string& hash) = 0;
    virtual std::vector<TorrentDownloadingInfo> GetTorrents() = 0;
    virtual TorrentDownloadingInfo GetTorrent(const std::string& hash) = 0;
    virtual void PauseTorrent(const std::string& hash);
};
