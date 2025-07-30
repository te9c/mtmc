#include <iostream>
#include <string>
#include <cstring>
#include <regex>

#include "httplib.h"
#include "pugixml.hpp"
#include "Torznab.h"
#include "../TorrentFileLink.h"

inline TorznabItem GetItemFromNode(const pugi::xml_node& itemNode) {
    TorznabItem item;
    item.title = itemNode.child("title").child_value();
    item.guid = itemNode.child("guid").child_value();
    item.comments = itemNode.child("comments").child_value();
    item.torrentLink = itemNode.child("link").child_value();
    for (auto attr : itemNode.children("torznab:attr")) {
        if (std::strcmp(attr.attribute("name").value(), "magneturl") == 0) {
            item.magnetUrl = attr.attribute("value").value();
        }
    }

    return item;
}

inline TorrentFileLink GetLinkFromItem(TorznabItem item) {
    TorrentFileLink link;
    link.title = item.title;
    link.pageUrl = item.guid;
    link.torrentLink = item.torrentLink;
    return link;
}

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

std::string Torznab::SearchXml(const std::string& query) {
    std::string q;
    q.reserve(query.size());

    for (auto c : query) {
        if (c == ' ') {
            q += "%20";
        } else {
            q.push_back(c);
        }
    }


    auto res = client_->Get(basepath_ + "/api?t=search&apikey=" + apikey_ + "&q=" + q);
    if (!res) {
        throw std::runtime_error(httplib::to_string(res.error()));
    }
    if (res->status != httplib::StatusCode::OK_200) {
        throw std::runtime_error("Expected 200 status code, got: " + std::to_string(res->status));
    }

    return res->body;
}

std::vector<TorrentFileLink> Torznab::SearchTorrentFiles(const std::string& query) {
    std::string xml = SearchXml(query);
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml.c_str());
    if (!result) {
        throw std::runtime_error("Failed to parse server response.\nError description: " + std::string(result.description()) + "\n");
    }

    if (pugi::xml_node err = doc.child("error")) {
        throw std::runtime_error("Error in api. Code: " + std::string(err.attribute("code").value()) +
                ". Description: " + std::string(err.attribute("description").value()));
    }

    std::vector<TorrentFileLink> links;
    for (auto itemNode : doc.child("rss").child("channel").children("item")) {
        links.push_back(GetLinkFromItem(GetItemFromNode(itemNode)));
    }

    return links;
}
