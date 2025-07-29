#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Indexer.h"
#include "httplib.h"
#include "../TorrentFileLink.h"

class Torznab : public Indexer {
public:
    Torznab(const std::string& baseurl, const std::string& apikey);
    std::vector<TorrentFileLink> SearchTorrentFiles(const std::string& query) override;

private:
    std::string SearchXml(const std::string& query);

    std::string basepath_;
    std::string apikey_;
    std::unique_ptr<httplib::Client> client_;
};
