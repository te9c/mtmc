#include <cctype>
#include <string>
#include <variant>

#include "Bencode.h"

Bencode::data get_integer(const std::string& str, int& idx);
Bencode::data get_string(const std::string& str, int& idx);
Bencode::data get_list(const std::string& str, int& idx);
Bencode::data get_dictionary(const std::string& str, int& idx);

Bencode::data get_data(const std::string& str, int& idx) {
    if (str[idx] == 'i') {
        return get_integer(str, idx);
    } else if (str[idx] == 'd') {
        return get_dictionary(str, idx);
    } else if (str[idx] == 'l') {
        return get_list(str, idx);
    } else if (str[idx] >= '0' && str[idx] <= '9') {
        return get_string(str, idx);
    } else {
        throw Bencode::BencodeDecoder::BencodeDecoderException("Expected opening symbol");
    }
}

Bencode::data Bencode::BencodeDecoder::decode(const std::string& input) {
    int index = 0;
    return get_data(input, index);
}


Bencode::data get_integer(const std::string& str, int& idx) {
    if (str[idx] == 'i') {
        ++idx;
    }

    Bencode::integer num = 0;
    char ch;
    while ((ch = str[idx++]) != 'e') {
        if (!std::isdigit(ch)) {
            throw Bencode::BencodeDecoder::BencodeDecoderException("expected number");
        }
        num *= 10;
        num += ch - '0';
    }
    return Bencode::data(num);
}
Bencode::data get_string(const std::string& str, int& idx) {
    auto res = Bencode::data(Bencode::string());

    Bencode::integer len = 0;
    char ch;
    while ((ch = str[idx++]) != ':') {
        if (!std::isdigit(ch)) {
            throw Bencode::BencodeDecoder::BencodeDecoderException("expected number");
        }
        len *= 10;
        len += ch - '0';
    }
    // TODO: check for out of bounds
    for (int i = 0; i < len; ++i) {
        ch = str[idx++];
        std::get<Bencode::string>(res).push_back(ch);
    }
    return res;
}

// TODO: error handling
Bencode::data get_dictionary(const std::string& str, int& idx) {
    auto res = Bencode::data(Bencode::dictionary());
    if (str[idx] == 'd') {
        ++idx;
    }
    char ch;
    while ((ch = str[idx]) != 'e') {
        auto key = get_string(str, idx);
        auto value = get_data(str, idx);

        std::get<Bencode::dictionary>(res)[std::get<Bencode::string>(key)] = value;
    }
    ++idx;
    return res;

}
Bencode::data get_list(const std::string& str, int& idx) {
    auto res = Bencode::data(Bencode::list());
    if (str[idx] == 'l') {
        ++idx;
    }
    char ch;
    while ((ch = str[idx]) != 'e') {
        std::get<Bencode::list>(res).push_back(get_data(str, idx));
    }
    ++idx;
    return res;
}

void encode_integer(const Bencode::integer& integer, std::string& result);
void encode_string(const Bencode::string& integer, std::string& result);
void encode_list(const Bencode::list& integer, std::string& result);
void encode_dicitonary(const Bencode::dictionary& integer, std::string& result);

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

void encode_data(const Bencode::data& data, std::string& result) {
    const auto visitor = overloads {
        [&result](const Bencode::integer& integer) { return encode_integer(integer, result); },
            [&result](const Bencode::string& str) { return encode_string(str, result); },
            [&result](const Bencode::list& list) { return encode_list(list, result); },
            [&result](const Bencode::dictionary& dictionary) { return encode_dicitonary(dictionary, result); }
    };
    std::visit(visitor, data);
}

std::string Bencode::BencodeEncoder::encode(const Bencode::data& input) {
    std::string result;
    encode_data(input, result);
    return result;
}

void encode_integer(const Bencode::integer& integer, std::string& result) {
    result += "i" + std::to_string(integer) + "e";
}
void encode_string(const Bencode::string& str, std::string& result) {
    result += std::to_string(str.size()) + ":" + str;
}
void encode_dicitonary(const Bencode::dictionary& dict, std::string& result) {
    result += "d";
    for (auto& [key, value] : dict) {
        encode_string(key, result);
        encode_data(value, result);
    }
    result += "e";
}
void encode_list(const Bencode::list& list, std::string& result) {
    result += "l";
    for (auto& value : list) {
        encode_data(value, result);
    }
    result += "e";
}

Bencode::data Bencode::decode(const std::string& input) {
    Bencode::BencodeDecoder decoder;
    return decoder.decode(input);
}
std::string Bencode::encode(const Bencode::data& input) {
    Bencode::BencodeEncoder encoder;
    return encoder.encode(input);
}
