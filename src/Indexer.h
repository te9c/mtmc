#pragma once

#include <vector>
#include <string>

#include "TorrentFileLink.h"

struct IndexerOptions {
    uint32_t searchLimit = 1;
};

class Indexer {
public:
    virtual std::vector<TorrentFileLink> SearchTorrentFiles(const std::string& query) = 0;
    IndexerOptions options;
};
