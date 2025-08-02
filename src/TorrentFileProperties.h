#pragma once

#include <string>
#include <vector>
#include <filesystem>

struct TorrentFileProperties {
    struct InternalFile {
        std::string path;
        size_t length;
    };

    std::string announce;
    std::string comment;
    std::vector<std::string> pieceHashes;
    size_t pieceLength;
    size_t length;
    std::string name;
    std::string infoHash;
    std::vector<std::string> announceList;
    std::vector<InternalFile> files;
};

TorrentFileProperties ParseTorrentFile(const std::filesystem::path& filename);
std::string CalculateSHA1(const std::string& data);
