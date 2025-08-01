#pragma once

#include <map>
#include <variant>
#include <vector>
#include <string>
#include <stdexcept>

namespace Bencode {
// types deduction:
// integer -> int64_t
// string -> std::string
// list -> std::vector<bencode::data>
// dictionary -> std::map<std::string, bencode::data>

struct data;
using integer = int64_t;
using string = std::string;
using list = std::vector<Bencode::data>;
using dictionary = std::map<std::string, Bencode::data>;
struct data : public std::variant<
              integer,
              string,
              list,
              dictionary> {};

class BencodeDecoder {
public:
    data decode(const std::string& input);

    class BencodeDecoderException : public std::runtime_error {
        public:
            BencodeDecoderException(const std::string& str) : std::runtime_error(str) {};
    };
};

class BencodeEncoder {
public:
    std::string encode(const Bencode::data& input);
};

Bencode::data decode(const std::string& input);

std::string encode(const Bencode::data& input);
};
