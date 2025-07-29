#pragma once

#include <string>

#include "../Indexer.h"
#include "httplib.h"

class Torznab : public Indexer {
public:
    Torznab(std::string baseurl, std::string apikey) : baseurl_(baseurl), apikey_(apikey), client_(baseurl) {};
private:
    std::string SearchXml(std::string query);

    std::string baseurl_;
    std::string apikey_;
    httplib::Client client_;
};
