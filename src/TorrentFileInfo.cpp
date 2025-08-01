#include "Bencode.h"
#include "TorrentFileInfo.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <openssl/sha.h>

TorrentInfo ParseTorrent(const std::filesystem::path& filename) {
    std::string input = "";

    {
        std::ifstream fstream;
        fstream.open(filename);
        std::stringstream buffer;
        buffer << fstream.rdbuf();
        input = buffer.str();
    }

    auto dict = std::get<Bencode::dictionary>(Bencode::decode(input));
    auto infoDict = std::get<Bencode::dictionary>(dict["info"]);

    TorrentInfo torrentFile;
    torrentFile.announce = std::get<Bencode::string>(dict["announce"]);
    torrentFile.comment = std::get<Bencode::string>(dict["comment"]);
    torrentFile.pieceLength = std::get<Bencode::integer>(infoDict["piece length"]);
    if (std::holds_alternative<Bencode::integer>(infoDict["length"]))
        torrentFile.length = std::get<Bencode::integer>(infoDict["length"]);
    torrentFile.name = std::get<Bencode::string>(infoDict["name"]);
    auto concatHash = std::get<Bencode::string>(infoDict["pieces"]);

    for (size_t i = 0; i < concatHash.size() / 20; ++i) {
        torrentFile.pieceHashes.push_back(std::string());
        for (size_t j = 0; j < 20; ++j) {
            torrentFile.pieceHashes.back().push_back(concatHash[i * 20 + j]);
        }
    }

    torrentFile.infoHash = CalculateSHA1(Bencode::encode(Bencode::data(infoDict)));

    // I guess for some backwards compatibility.
    torrentFile.announceList.push_back(torrentFile.announce);

    if (std::holds_alternative<Bencode::list>(dict["announce-list"])) {
        for (const auto& data : std::get<Bencode::list>(dict["announce-list"])) {
            if (!std::holds_alternative<Bencode::list>(data))
                continue;
            for (const auto& s_data : std::get<Bencode::list>(data)) {
                if (!std::holds_alternative<Bencode::string>(s_data))
                    continue;
                torrentFile.announceList.push_back(std::get<Bencode::string>(s_data));
            }
        }
    }
    if (std::holds_alternative<Bencode::list>(infoDict["files"])) {
        torrentFile.length = 0;
        torrentFile.files.reserve(std::get<Bencode::list>(infoDict["files"]).size());
        for (const auto& d : std::get<Bencode::list>(infoDict["files"])) {
            auto di = std::get<Bencode::dictionary>(d);
            torrentFile.files.emplace_back();
            torrentFile.files.back().length = std::get<Bencode::integer>(di["length"]);
            torrentFile.length += torrentFile.files.back().length;
            for (const auto& p : std::get<Bencode::list>(di["path"])) {
                if (torrentFile.files.back().path.size()) {
                    torrentFile.files.back().path.push_back('/');
                }
                torrentFile.files.back().path += std::get<Bencode::string>(p);
            }
        }
    }

    if (torrentFile.files.empty()) {
        torrentFile.files.emplace_back(torrentFile.name, torrentFile.length);
    }

    return torrentFile;
}

std::string CalculateSHA1(const std::string& data) {
    std::string result(20, 0);
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), reinterpret_cast<unsigned char*>(result.data()));
    return result;
}
