#include <iostream>
#include <string>
#include <regex>

#include "Torznab.h"
#include "httplib.h"
#include "../TorrentFileLink.h"

Torznab::Torznab(const std::string& baseurl, const std::string& apikey) : apikey_(apikey) {
    std::regex pattern(R"((https?:\/\/)?([a-zA-Z0-9.]+)(?:\:(\d+))?(?:\/(.*))?$)");
    std::smatch matches;
    if (!std::regex_match(baseurl, matches, pattern)) {
        throw std::runtime_error("could not match url: " + baseurl + " with url regex.");
    }

    std::string proto = matches[1];
    std::string host = matches[2];
    std::string port = matches[3];
    std::string path = "/" + std::string(matches[4]);
    while (path.size() && path.back() == '/') {
        path.pop_back();
    }

    std::string url = proto + host;
    if (!port.empty()) {
        url += ":" + port;
    }

    basepath_ = path;
    client_ = std::make_unique<httplib::Client>(url);
}

std::string Torznab::SearchXml(std::string query) {
    auto res = client_->Get(basepath_ + "/api?t=search&apikey=" + apikey_ + "&q=" + query);
    if (!res) {
        throw std::runtime_error(httplib::to_string(res.error()));
    }
    if (res->status != httplib::StatusCode::OK_200) {
        throw std::runtime_error("Expected 200 status code, got: " + std::to_string(res->status));
    }

    return res->body;
}

std::vector<TorrentFileLink> Torznab::SearchTorrentFiles(const std::string& query) {
    throw std::runtime_error("not implemented");
} 
