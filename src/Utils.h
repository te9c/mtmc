#pragma once

#include <regex>
#include <string>
#include <stdexcept>

// I know that having utils header is
// generally bad practice but at this
// moment I think it would be the best
// to hold unsorted functions there than
// creating separete header file for each
// of them.

namespace utils {

struct Url {
    std::string proto;
    std::string host;
    std::string port;
    std::string path;

    // proto + host + port (if exists)
    std::string GetBase() const {
        std::string u = proto + host;
        if (!port.empty()) {
            u += ":" + port;
        }
        return u;
    }
};

inline Url ParseUrl(const std::string& url) {
    std::regex pattern(R"((https?:\/\/)?([a-zA-Z0-9.]+)(?:\:(\d+))?(?:\/(.*))?$)");
    std::smatch matches;
    if (!std::regex_match(url, matches, pattern)) {
        throw std::runtime_error("could not match url: " + url + " with url regex.");
    }

    Url res;

    res.proto = matches[1];
    if (res.proto.empty()) {
        res.proto = "http://";
    }
    res.host = matches[2];
    res.port = matches[3];
    res.path = "/" + std::string(matches[4]);

    while (res.path.size() && res.path.back() == '/') {
        res.path.pop_back();
    }

    return res;
}

}
