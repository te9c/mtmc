#include <regex>

#include "Qbittorrent.h"
#include "../Utils.h"
#include "../Errors.h"
#include "httplib.h"

Qbittorrent::Qbittorrent(const std::string& baseurl, const std::string& username, const std::string& password) : username_(username), password_(password) {
    url_ = utils::ParseUrl(baseurl);

    client_ = std::make_unique<httplib::Client>(url_.GetBase());
}

void Qbittorrent::Authenticate() {
    httplib::Headers headers = {
        { "Referer", url_.GetBase() },
    };
    auto res = client_->Post(url_.path + "/api/v2/auth/login", headers, "username=" + username_ + "&password=" + password_, "application/x-www-form-urlencoded");
    if (!res) {
        throw std::runtime_error(httplib::to_string(res.error()));
    }
    if (res->status != httplib::StatusCode::OK_200) {
        throw AuthenticationError("Response status (" + std::to_string(res->status) + ") is not 200. Could be banned because of too many attempts.");
    }
    if (!res->has_header("set-cookie")) {
        throw InvalidCredentialsError("Invalid credentials.");
    }
    std::string cookie = res->get_header_value("set-cookie");

    std::regex pattern(R"(SID=([a-zA-Z0-9/_+-]*);)");
    std::smatch sm;
    if (!std::regex_search(cookie, sm, pattern)) {
        throw ApiError("Can't match cookie string (" + cookie + ") from server to regex.");
    }
    cookie_ = sm[1];
}

void Qbittorrent::AddTorrent(const DownloadRequest& request) {
    throw std::runtime_error("not implemented");
}
void Qbittorrent::DeleteTorrent(const std::string& hash) {
    throw std::runtime_error("not implemented");
}
std::vector<TorrentInfo> Qbittorrent::GetTorrents() {
    throw std::runtime_error("not implemented");
}
TorrentInfo Qbittorrent::GetTorrent(const std::string& hash) {
    throw std::runtime_error("not implemented");
}
std::vector<InternalTorrentFile> Qbittorrent::GetTorrentFiles(const std::string& hash) {
    throw std::runtime_error("not implemented");
}
void Qbittorrent::StartDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) {
    throw std::runtime_error("not implemented");
}
void Qbittorrent::StopDownloadingFiles(const std::string& hash, const std::vector<std::string>& files) {
    throw std::runtime_error("not implemented");
}
void Qbittorrent::PauseTorrent(const std::string& hash) {
    throw std::runtime_error("not implemented");
}
