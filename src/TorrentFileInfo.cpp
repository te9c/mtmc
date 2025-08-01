#include "Bencode.h"
#include "TorrentFileInfo.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <openssl/sha.h>

TorrentFileInfo ParseTorrentFile(const std::filesystem::path& filename) {
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

    TorrentFileInfo torrentFile;
    torrentFile.announce = std::get<Bencode::string>(dict["announce"]);
    torrentFile.comment = std::get<Bencode::string>(dict["comment"]);
    torrentFile.pieceLength = std::get<Bencode::integer>(infoDict["piece length"]);
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
        for (auto data : std::get<Bencode::list>(dict["announce-list"])) {
            if (!std::holds_alternative<Bencode::list>(data))
                continue;
            for (auto s_data : std::get<Bencode::list>(data)) {
                if (!std::holds_alternative<Bencode::string>(s_data))
                    continue;
                torrentFile.announceList.push_back(std::get<Bencode::string>(s_data));
            }
        }
    }

    return torrentFile;
}

std::string CalculateSHA1(const std::string& data) {
    std::string result(20, 0);
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), reinterpret_cast<unsigned char*>(result.data()));
    return result;
}
