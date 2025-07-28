#pragma once

#include <string>

#include "../Indexer.h"

class Torznab : public Indexer {
public:
    Torznab(std::string baseurl, std::string apikey) : baseurl_(baseurl), apikey_(apikey) {};
private:
    std::string baseurl_;
    std::string apikey_;
};
