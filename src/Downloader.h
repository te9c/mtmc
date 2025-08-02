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

struct DownloadRequest {
    std::filesystem::path torrentFilePath;
    std::filesystem::path savePath;
    bool paused = true;
};

struct TorrentInfo {
    std::string name;
    std::string hash;
    std::filesystem::path savedPath;
    uint64_t downloadedBytes;
    uint64_t totalSizeBytes;
};

// This is for files that
// located inside of a torrent,
// not the torrent file itself.
struct InternalTorrentFile {
    std::string path;
    uint64_t size;
    float progress;
    bool inProgress;
};

struct DownloaderError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Downloader {
public:
    virtual void AddTorrent(const DownloadRequest& request) = 0;
    virtual void DeleteTorrent(const std::string& hash) = 0;
    virtual std::vector<TorrentInfo> GetTorrents() = 0;
    virtual TorrentInfo GetTorrent(const std::string& hash) = 0;
    virtual std::vector<InternalTorrentFile> GetTorrentFiles(const std::string& hash) = 0;
    virtual void StartDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) = 0;
    virtual void StopDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) = 0;
    virtual void PauseTorrent(const std::string& hash) = 0;
};
