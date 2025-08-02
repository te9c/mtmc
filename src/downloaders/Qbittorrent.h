#pragma once

#include <string>
#include <vector>

#include "../Downloader.h"
#include "../Utils.h"
#include "httplib.h"

#define MIN_API_VERSION v5.0.5

class Qbittorrent : public Downloader {
public:

    Qbittorrent(const std::string& baseurl, const std::string& username, const std::string& password);

    std::string GetApplicationVersion();
    std::string GetApiVersion();

    void AddTorrent(const DownloadRequest& request) override;
    void DeleteTorrent(const std::string& hash) override;
    std::vector<TorrentInfo> GetTorrents() override;
    TorrentInfo GetTorrent(const std::string& hash) override;
    std::vector<InternalTorrentFile> GetTorrentFiles(const std::string& hash) override;
    void StartDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) override;
    void StopDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) override;
    void PauseTorrent(const std::string& hash) override;

private:
    void authenticate();
    httplib::Headers get_default_headers();

    utils::Url url_;
    std::string username_;
    std::string password_;
    std::string cookie_;
    std::unique_ptr<httplib::Client> client_;
};
